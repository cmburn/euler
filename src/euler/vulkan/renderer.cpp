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
	if (vec.empty()) return vec;
	log->info("Available SDL Vulkan instance extensions:");
	for (const auto s : vec) log->info("\t- %s", s);
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
		.apiVersion = vk::ApiVersion12,
	};
	static constexpr const char *validation_layers[] = {
		"VK_LAYER_KHRONOS_validation",
	};
	auto sdl_exts = instance_extensions(log);
	sdl_exts.push_back("VK_KHR_push_descriptor");

	const vk::InstanceCreateInfo create_info {
		.pApplicationInfo = &app_info,
		.enabledLayerCount = 1,
		.ppEnabledLayerNames = validation_layers,
		.enabledExtensionCount = static_cast<uint32_t>(sdl_exts.size()),
		.ppEnabledExtensionNames = sdl_exts.data(),
	};

	return vk::raii::Instance(context, create_info);
}

euler::vulkan::Renderer::Renderer(const util::Reference<Surface> &surface,
    const Config &config)
    : _instance(make_instance(surface->log(), _context, config))
    , _config(config)
    , _surface(surface)
    , _log(surface->log())
    , _physical_device(create_physical_device())
    , _device(create_device())
{
}

void
euler::vulkan::Renderer::frame(const std::function<void()> &fn)
{
	try {
		start_frame();
		fn();
		end_frame();
	} catch (...) {
		end_frame();
		throw;
	}
}

euler::vulkan::Renderer::~Renderer() { renderer_semaphore.release(); }

void
euler::vulkan::Renderer::start_frame()
{
	RenderTarget rt = {};
	_surface->start_frame(rt);
}

void
euler::vulkan::Renderer::end_frame()
{
	_frame_semaphore.release();
}

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
		_log->info("Found device with ID %d (%s)", properties.deviceID,
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
		    "Unable to find user-specified graphics device with ID %d",
		    _config.preferred_gpu.value());
	}
	if (discrete_device.has_value()) return discrete_device.value();
	return devices.at(0);
}

euler::vulkan::PhysicalDevice
euler::vulkan::Renderer::create_physical_device()
{
	auto pdev = select_physical_device();
	return { this, std::move(pdev) };
}

vk::raii::Device
euler::vulkan::Renderer::select_device()
{
	static constexpr std::array exts = {
		"VK_KHR_swapchain",
	};
	static constexpr float priority = 1.0f;
	const auto &pdev = _physical_device.physical_device();
	auto props = pdev.getQueueFamilyProperties();
	std::array queue_info = {
		(vk::DeviceQueueCreateInfo) {
		    .queueFamilyIndex = _graphics_queue_index,
		    .queueCount = 1,
		    .pQueuePriorities = &priority,
		},
		(vk::DeviceQueueCreateInfo) {
		    .queueFamilyIndex = _present_queue_index,
		    .queueCount = 1,
		    .pQueuePriorities = &priority,
		},
#ifdef EULER_ENABLE_COMPUTE_QUEUE
		(vk::DeviceQueueCreateInfo) {
		    .queueFamilyIndex = _compute_queue_index,
		    .queueCount = 1,
		    .pQueuePriorities = &priority,
		},
#endif
	};

	static constexpr vk::PhysicalDeviceFeatures features = {};

	const vk::DeviceCreateInfo create_info = {
		.queueCreateInfoCount
		= static_cast<uint32_t>(queue_info.size()),
		.pQueueCreateInfos = queue_info.data(),
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = static_cast<uint32_t>(exts.size()),
		.ppEnabledExtensionNames = exts.data(),
		.pEnabledFeatures = &features,
	};

	return _physical_device.physical_device().createDevice(create_info);
}

euler::vulkan::Device
euler::vulkan::Renderer::create_device()
{
	return Device { util::Reference(this), select_device() };
}
