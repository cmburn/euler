/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/renderer.h"

#include <VK2D/Logger.h>
#include <VK2D/VK2D.h>

#include "euler/util/logger.h"
#include "euler/vulkan/surface.h"

namespace util = euler::util;

static std::binary_semaphore renderer_semaphore(1);

static VK2DLogSeverity
to_vk2d(const util::Logger::Severity severity)
{
	switch (severity) {
	case util::Logger::Severity::Debug: return VK2D_LOG_SEVERITY_DEBUG;
	case util::Logger::Severity::Info: return VK2D_LOG_SEVERITY_INFO;
	case util::Logger::Severity::Warn: return VK2D_LOG_SEVERITY_WARN;
	case util::Logger::Severity::Error: return VK2D_LOG_SEVERITY_ERROR;
	case util::Logger::Severity::Fatal: return VK2D_LOG_SEVERITY_FATAL;
	case util::Logger::Severity::Unknown: [[fallthrough]];
	default: return VK2D_LOG_SEVERITY_UNKNOWN;
	}
}

static util::Logger::Severity
from_vk2d(const VK2DLogSeverity severity)
{
	switch (severity) {
	case VK2D_LOG_SEVERITY_DEBUG: return util::Logger::Severity::Debug;
	case VK2D_LOG_SEVERITY_INFO: return util::Logger::Severity::Info;
	case VK2D_LOG_SEVERITY_WARN: return util::Logger::Severity::Warn;
	case VK2D_LOG_SEVERITY_ERROR: return util::Logger::Severity::Error;
	case VK2D_LOG_SEVERITY_FATAL: return util::Logger::Severity::Fatal;
	case VK2D_LOG_SEVERITY_UNKNOWN: [[fallthrough]];
	default: return util::Logger::Severity::Unknown;
	}
}

euler::vulkan::Renderer::~Renderer() { renderer_semaphore.release(); }

void
euler::vulkan::Renderer::initialize(const util::Reference<Surface> &surface)
{
	_log->info("Initializing Vulkan renderer");
	if (!renderer_semaphore.try_acquire()) {
		_log->error("Attempted to initialize multiple renderers");
		throw std::runtime_error("Multiple renderer initialization");
	}
	_initialized = true;
	_surface = surface.weaken();
	static constexpr VK2DRendererConfig config = {
		.msaa = VK2D_MSAA_16X,
		.screenMode = VK2D_SCREEN_MODE_VSYNC,
		.filterMode = VK2D_FILTER_TYPE_NEAREST,
	};
	static constexpr VK2DStartupOptions startup_options = {
		.enableDebug = true,
		.stdoutLogging = true,
		.quitOnError = false,
		.errorFile = nullptr,
		.maxTextures = std::numeric_limits<uint16_t>::max(),
		.enableNuklear = true,
		.vramPageSize = 0,
	};

	/* ReSharper disable CppParameterMayBeConstPtrOrRef */
	_vk2d_logger = new VK2DLogger {
		.log =
		    [](void *ctx, const VK2DLogSeverity severity,
			const char *message) {
			    const auto log
				= util::Reference<util::Logger>::unwrap(ctx);
			    log->log(from_vk2d(severity), "{}", message);
		    },
		.destroy =
		    [](void *ctx) {
			    auto log
				= util::Reference<util::Logger>::unwrap(ctx);
			    log = nullptr;
			    (void)log;
		    },
		.severityFn =
		    [](void *ctx) {
			    const auto log
				= util::Reference<util::Logger>::unwrap(ctx);
			    return to_vk2d(log->severity());
		    },
		.context = _log.wrap(),
	};
	vk2dSetLogger(_vk2d_logger);

	/* ReSharper restore CppParameterMayBeConstPtrOrRef */
	vk2dRendererInit(surface->window(), config, &startup_options);
	_log->info("Vulkan renderer initialized");
	surface->set_renderer(util::Reference(this));
}

nk_context *
euler::vulkan::Renderer::gui_context()
{
	return vk2dGuiContext();
}

const nk_context *
euler::vulkan::Renderer::gui_context() const
{
	return vk2dGuiContext();
}
util::Reference<euler::vulkan::Surface>
euler::vulkan::Renderer::surface() const
{
	return _surface.strengthen();
}