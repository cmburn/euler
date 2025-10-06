/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/renderer.h"

#include "SDL3/SDL_vulkan.h"

namespace util = euler::util;

static std::binary_semaphore renderer_semaphore(1);

static const std::vector<const char *> &
instance_extensions(const util::Reference<util::Logger> &log)
{
	static bool initialized = false;
	static std::mutex vec_mutex;
	static std::vector<const char *> vec;
	/* could cause trouble if we're trying to create two at once */
	std::lock_guard lock(vec_mutex);
	if (initialized) return vec;
	uint32_t count;
	const auto *exts = SDL_Vulkan_GetInstanceExtensions(&count);
	if (exts == nullptr) {
		if (log != nullptr) {
			log->fatal(
			    "Failed to get SDL Vulkan instance extensions");
		}
		fprintf(stderr,
		    "Failed to get SDL Vulkan instance extensions\n");
		std::exit(EXIT_FAILURE);
	}
	vec.reserve(count);
	for (uint32_t i = 0; i < count; i++) vec.emplace_back(exts[i]);
	initialized = true;
	return vec;
}

static vk::raii::Instance
make_instance(const util::Reference<util::Logger> &log,
    const vk::raii::Context &context,
    const euler::vulkan::Renderer::Config &config)
{
	if (!renderer_semaphore.try_acquire()) {
		throw std::runtime_error("Only one vulkan::Renderer may be "
					 "active at any given time");
	}
	const vk::ApplicationInfo app_info {
		.pApplicationName = config.application.c_str(),
		.applicationVersion = config.version.to_vulkan(),
		.pEngineName = "euler",
		.engineVersion = util::version().to_vulkan(),
		.apiVersion = vk::ApiVersion14,
	};
	static constexpr const char *validation_layers[] = {
		"VK_LAYER_KHRONOS_validation",
	};
	auto sdl_exts = instance_extensions(log);
	// sdl_exts.push_back("VK_KHR_push_descriptor");

	const vk::InstanceCreateInfo create_info {
		.pApplicationInfo = &app_info,
		.enabledLayerCount = 1,
		.ppEnabledLayerNames = validation_layers,
		.enabledExtensionCount = static_cast<uint32_t>(sdl_exts.size()),
		.ppEnabledExtensionNames = sdl_exts.data(),
	};

	auto inst = vk::raii::Instance(context, create_info);
	return inst;
}

euler::vulkan::Renderer::Config
euler::vulkan::Renderer::default_config()
{
	return Config {};
}

euler::vulkan::Renderer::Renderer(const Config &config)
    : _instance(nullptr)
    , _config(config)
{
}

std::optional<euler::vulkan::Renderer::ShaderData>
euler::vulkan::Renderer::shader_data(std::string_view key)
{
	auto builtin = load_builtin_shader(key);
	if (builtin.has_value()) return *builtin;
	if (!_runtime_shaders.contains(std::string(key))) return std::nullopt;
	return _runtime_shaders.at(std::string(key));
}

std::optional<vk::raii::ShaderModule>
euler::vulkan::Renderer::load_shader(std::string_view key)
{
	auto data = shader_data(key);
	if (!data.has_value()) return std::nullopt;
	const vk::ShaderModuleCreateInfo ci = {
		.codeSize = data->size(),
		.pCode = reinterpret_cast<const uint32_t *>(data->data()),
	};
	return _device.device().createShaderModule(ci);
}

void
euler::vulkan::Renderer::initialize(const util::Reference<Surface> &surface)
{
	_context = make_context();
	_instance = make_instance(surface->log(), _context, config());
	_surface = surface;
	_log = surface->log();
	_physical_device = create_physical_device();
	_log->info("Using queue family {} for graphics",
	    _physical_device.graphics_family());
	_log->info("Using queue family {} for compute",
	    _physical_device.compute_family());
	std::construct_at(&_device, create_device());
	surface->initialize_vulkan(util::Reference(this));
}

euler::vulkan::Renderer::~Renderer() { renderer_semaphore.release(); }

vk::raii::PhysicalDevice
euler::vulkan::Renderer::select_physical_device()
{
	auto devices = _instance.enumeratePhysicalDevices();
	if (devices.empty())
		_log->fatal("Failed to enumerate physical devices");

	/* prefer a discrete gpu */
	std::optional<vk::raii::PhysicalDevice> discrete_device;
	std::optional<vk::raii::PhysicalDevice> selected_device;

	for (const auto &d : devices) {
		auto properties = d.getProperties();
		_log->info("Found device with ID {} ({})", properties.deviceID,
		    properties.deviceName.data());
		if (_config.preferred_gpu.has_value()
		    && properties.deviceID == _config.preferred_gpu.value()) {
			selected_device = std::move(d);
			break;
		}
		if (properties.deviceType
			== vk::PhysicalDeviceType::eDiscreteGpu
		    && !discrete_device.has_value()) {
			discrete_device = std::move(d);
		}
	}
	if (selected_device.has_value()) return selected_device.value();
	if (_config.preferred_gpu.has_value()) {
		_log->info(
		    "Unable to find user-specified graphics device with ID {}",
		    _config.preferred_gpu.value());
	}
	if (discrete_device.has_value()) return discrete_device.value();
	return devices.at(0);
}

euler::vulkan::PhysicalDevice
euler::vulkan::Renderer::create_physical_device()
{
	auto pdev = select_physical_device();
	log()->info("Using device: {}", pdev.getProperties().deviceName.data());
	return { this, std::move(pdev) };
}

vk::raii::Device
euler::vulkan::Renderer::select_device()
{
	static constexpr std::array EXTS = {
		vk::KHRDynamicRenderingExtensionName,
		vk::KHRPushDescriptorExtensionName,
		vk::KHRSpirv14ExtensionName,
		vk::KHRSwapchainExtensionName,
	};

	static constexpr float PRIORITY = 1.0f;
	const auto &pdev = _physical_device.physical_device();
	auto props = pdev.getQueueFamilyProperties();
	std::array queue_info = {
		(vk::DeviceQueueCreateInfo) {
		    .queueFamilyIndex = _physical_device.graphics_family(),
		    .queueCount = 1,
		    .pQueuePriorities = &PRIORITY,
		},
		(vk::DeviceQueueCreateInfo) {
		    .queueFamilyIndex = _physical_device.compute_family(),
		    .queueCount = 1,
		    .pQueuePriorities = &PRIORITY,
		},
	};

	uint32_t queue_size = queue_info.size();
	if (_physical_device.compute_family() == PhysicalDevice::NO_QUEUE)
		--queue_size;

	// static const vk::StructureChain<vk::PhysicalDeviceFeatures2,
	//     vk::PhysicalDeviceVulkan12Features,
	//     vk::PhysicalDeviceDynamicRenderingFeatures,
	//     vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
	//     FEATURE_CHAIN = {
	// 	    {},
	// 	    {},
	// 	    {},
	// 	    { .extendedDynamicState = true },
	//     };

	static const vk::StructureChain<vk::PhysicalDeviceFeatures2,
	    vk::PhysicalDeviceVulkan12Features,
	    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
	    FEATURE_CHAIN = {
		    {},
		    {},
		    { .extendedDynamicState = true },
	    };

	const vk::DeviceCreateInfo create_info = {
		.pNext = &FEATURE_CHAIN.get<vk::PhysicalDeviceFeatures2>(),
		.queueCreateInfoCount = queue_size,
		.pQueueCreateInfos = queue_info.data(),
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = static_cast<uint32_t>(EXTS.size()),
		.ppEnabledExtensionNames = EXTS.data(),
	};

	return pdev.createDevice(create_info);
}

euler::vulkan::Device
euler::vulkan::Renderer::create_device()
{
	return Device { util::Reference(this), select_device() };
}

vk::raii::Context
euler::vulkan::Renderer::make_context()
{
	return vk::raii::Context();
}

const uint8_t *
euler_vulkan_renderer_shader_data(euler_vulkan_renderer *renderer,
    const char *key, size_t *size_out)
{
	const auto out = renderer->shader_data(key);
	if (!out.has_value()) {
		*size_out = 0;
		return nullptr;
	}
	*size_out = out->size();
	return out->data();
}