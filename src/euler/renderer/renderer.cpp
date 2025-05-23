/* SPDX-License-Identifier: ISC */

#include "euler/renderer/renderer.h"

#include "VK2D/Gui.h"
#include "VK2D/Logger.h"
#include "VK2D/Renderer.h"
#include "VK2D/Structs.h"

#include "euler/renderer/window.h"
#include "euler/util/logger.h"

using LogReference = Euler::Util::Reference<Euler::Util::Logger>;
using Severity = Euler::Util::Logger::Severity;

inline std::strong_ordering
operator<=>(VK2DLogSeverity lhs, Severity rhs)
{
	const auto li = static_cast<std::underlying_type_t<Severity>>(lhs);
	const auto ri = static_cast<std::underlying_type_t<Severity>>(rhs);
	return li <=> ri;
}

static VK2DMSAA
to_vk2d(const Euler::Renderer::Renderer::MSAA msaa)
{
	switch (msaa) {
	case Euler::Renderer::Renderer::MSAA::X1: return VK2D_MSAA_1X;
	case Euler::Renderer::Renderer::MSAA::X2: return VK2D_MSAA_2X;
	case Euler::Renderer::Renderer::MSAA::X4: return VK2D_MSAA_4X;
	case Euler::Renderer::Renderer::MSAA::X8: return VK2D_MSAA_8X;
	case Euler::Renderer::Renderer::MSAA::X16: return VK2D_MSAA_16X;
	case Euler::Renderer::Renderer::MSAA::X32: return VK2D_MSAA_32X;
	}
	assert(false);
}

static VK2DScreenMode
to_vk2d(const Euler::Renderer::Renderer::ScreenMode screen_mode)
{
	switch (screen_mode) {
	case Euler::Renderer::Renderer::ScreenMode::Immediate:
		return VK2D_SCREEN_MODE_IMMEDIATE;
	case Euler::Renderer::Renderer::ScreenMode::VSync:
		return VK2D_SCREEN_MODE_VSYNC;
	case Euler::Renderer::Renderer::ScreenMode::TripleBuffer:
		return VK2D_SCREEN_MODE_TRIPLE_BUFFER;
	}
	assert(false);
}

static VK2DFilterType
to_vk2d(const Euler::Renderer::Renderer::FilterType filter_type)
{
	switch (filter_type) {
	case Euler::Renderer::Renderer::FilterType::Linear:
		return VK2D_FILTER_TYPE_LINEAR;
	case Euler::Renderer::Renderer::FilterType::Nearest:
		return VK2D_FILTER_TYPE_NEAREST;
	}
	assert(false);
}

inline VK2DLogSeverity
to_vk2d(Severity severity)
{
	assert(severity >= VK2D_LOG_SEVERITY_DEBUG
	    && severity <= VK2D_LOG_SEVERITY_INFO);
	return static_cast<VK2DLogSeverity>(severity);
}

inline VK2DRendererConfig
to_vk2d(const Euler::Renderer::Renderer::Config &config)
{
	return (VK2DRendererConfig) {
		.msaa = to_vk2d(config.msaa),
		.screenMode = to_vk2d(config.screen_mode),
		.filterMode = to_vk2d(config.filter_type),
	};
}

inline Severity
from_vk2d(VK2DLogSeverity severity)
{
	assert(severity >= Severity::Debug && severity <= Severity::Unknown);
	return static_cast<Severity>(severity);
}

inline Euler::Renderer::Renderer::Config
from_vk2d(VK2DRendererConfig config)
{
	return {
		.msaa
		= static_cast<Euler::Renderer::Renderer::MSAA>(config.msaa),
		.screen_mode
		= static_cast<Euler::Renderer::Renderer::ScreenMode>(
		    config.screenMode),
		.filter_type
		= static_cast<Euler::Renderer::Renderer::FilterType>(
		    config.filterMode),
	};
}

Euler::Renderer::Renderer::~Renderer() { vk2dRendererQuit(); }

Euler::Renderer::Renderer::Config
Euler::Renderer::Renderer::default_config()
{
	static constexpr Config DEFAULT_CONFIG = {};
	return DEFAULT_CONFIG;
}

Euler::Renderer::Renderer::Renderer(Util::Reference<Util::Logger> log,
    const Config &config, std::vector<Font> &&fonts)
    : _config(config)
{
#ifdef EULER_MAX_TEXTURES
	static constexpr size_t MAX_TEXTURES = EULER_MAX_TEXTURES;
#else
	static constexpr size_t MAX_TEXTURES = (1 << 14) - 1;
#endif
	assert(log != nullptr);
	_window = Util::make_reference<Window>();
	const VK2DRendererConfig vk2d_config = {
		.msaa = to_vk2d(config.msaa),
		.screenMode = VK2D_SCREEN_MODE_VSYNC,
		.filterMode = VK2D_FILTER_TYPE_LINEAR,
	};
	VK2DStartupOptions startup_options = {
		.enableDebug = true,
		.stdoutLogging = true,
		.quitOnError = true,
		.errorFile = nullptr,
		.maxTextures = MAX_TEXTURES,
		.enableNuklear = true,
		.vramPageSize = 0,
	};
	_logger_context = Util::make_reference<LoggerContext>(log);
	vk2dSetLogger(_logger_context->ptr);
	vk2dRendererInit(_window->window(), vk2d_config, &startup_options);
	_gui = Util::make_reference<Gui>(_log, _window, std::move(fonts));
	_renderer = vk2dRendererGetPointer();
}

Euler::Renderer::Renderer::Config
Euler::Renderer::Renderer::config() const
{
	return from_vk2d(vk2dRendererGetConfig());
}

void
Euler::Renderer::Renderer::set_config(const Config &config)
{
	const auto vk2d_config = to_vk2d(config);
	vk2dRendererSetConfig(vk2d_config);
}

/* ReSharper disable once CppMemberFunctionMayBeStatic */
void
Euler::Renderer::Renderer::reset_swapchain()
{
	vk2dRendererResetSwapchain();
}

bool
Euler::Renderer::Renderer::loop(std::function<bool(SDL_Event *)> input_callback,
    std::function<void()> gui_callback)
{
	return true;
}

Euler::Renderer::Renderer::LoggerContext::LoggerContext(
    const Util::Reference<Util::Logger> &logger)
{
	using LogCtxRef = Util::Reference<LoggerContext>;
	log = logger;
	ptr = new VK2DLogger {
		.log =
		    [](void *context, const VK2DLogSeverity severity,
			const char *message) {
			    const auto ref = LogCtxRef::unwrap(context);
			    const auto sev = from_vk2d(severity);
			    ref->log->log(sev, "{}", message);
		    },
		.destroy =
		    [](void *context) {
			    const auto ref = LogCtxRef::unwrap(context);
			    ref.decrement();
		    },
		.severityFn =
		    [](void *context) {
			    const auto ref = LogCtxRef::unwrap(context);
			    return to_vk2d(ref->log->level());
		    },
		.context = Util::Reference(this).wrap(),
	};
}

Euler::Renderer::Renderer::LoggerContext::~LoggerContext()
{
	delete ptr;
	ptr = nullptr;
}