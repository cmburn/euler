/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#include "euler/render/renderer.h"

using LogReference = euler::util::Reference<euler::util::Logger>;
using Severity = euler::util::Logger::Severity;

inline std::strong_ordering
operator<=>(VK2DLogSeverity lhs, Severity rhs)
{
	const auto li = static_cast<std::underlying_type_t<Severity>>(lhs);
	const auto ri = static_cast<std::underlying_type_t<Severity>>(rhs);
	return li <=> ri;
}

static VK2DMSAA
to_vk2d(const euler::graphics::Renderer::MSAA msaa)
{
	switch (msaa) {
	case euler::graphics::Renderer::MSAA::X1: return VK2D_MSAA_1X;
	case euler::graphics::Renderer::MSAA::X2: return VK2D_MSAA_2X;
	case euler::graphics::Renderer::MSAA::X4: return VK2D_MSAA_4X;
	case euler::graphics::Renderer::MSAA::X8: return VK2D_MSAA_8X;
	case euler::graphics::Renderer::MSAA::X16: return VK2D_MSAA_16X;
	case euler::graphics::Renderer::MSAA::X32: return VK2D_MSAA_32X;
	}
	assert(false);
}

static VK2DScreenMode
to_vk2d(const euler::graphics::Renderer::ScreenMode screen_mode)
{
	switch (screen_mode) {
	case euler::graphics::Renderer::ScreenMode::Immediate:
		return VK2D_SCREEN_MODE_IMMEDIATE;
	case euler::graphics::Renderer::ScreenMode::VSync:
		return VK2D_SCREEN_MODE_VSYNC;
	case euler::graphics::Renderer::ScreenMode::TripleBuffer:
		return VK2D_SCREEN_MODE_TRIPLE_BUFFER;
	}
	assert(false);
}

static VK2DFilterType
to_vk2d(const euler::graphics::Renderer::FilterType filter_type)
{
	switch (filter_type) {
	case euler::graphics::Renderer::FilterType::Linear:
		return VK2D_FILTER_TYPE_LINEAR;
	case euler::graphics::Renderer::FilterType::Nearest:
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
to_vk2d(const euler::graphics::Renderer::Config &config)
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

inline euler::graphics::Renderer::Config
from_vk2d(VK2DRendererConfig config)
{
	return {
		.msaa
		= static_cast<euler::graphics::Renderer::MSAA>(config.msaa),
		.screen_mode
		= static_cast<euler::graphics::Renderer::ScreenMode>(
		    config.screenMode),
		.filter_type
		= static_cast<euler::graphics::Renderer::FilterType>(
		    config.filterMode),
	};
}

euler::graphics::Renderer::~Renderer() { vk2dRendererQuit(); }

euler::graphics::Renderer::Config
euler::graphics::Renderer::default_config()
{
	static constexpr Config DEFAULT_CONFIG = {};
	return DEFAULT_CONFIG;
}

euler::graphics::Renderer::Renderer(const util::Reference<util::Logger> &log,
    const Config &config, std::vector<Font> &&fonts)
    : _log(log)
    , _config(config)
{
#ifdef EULER_MAX_TEXTURES
	static constexpr size_t MAX_TEXTURES = EULER_MAX_TEXTURES;
#else
	static constexpr size_t MAX_TEXTURES = (1 << 14) - 1;
#endif
	assert(log != nullptr);
	_logger_context = util::make_reference<LoggerContext>(log);
	vk2dSetLogger(_logger_context->ptr);
	_window = util::make_reference<Window>();
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
	auto result = vk2dRendererInit(_window->window(), vk2d_config, &startup_options);
	if (result != VK2D_SUCCESS) {
		_log->fatal("Failed to initialize renderer: VK2D code {}",
		    static_cast<int>(result));
	}
	_gui = util::make_reference<Gui>(_log, _window, std::move(fonts));
	_renderer = vk2dRendererGetPointer();
}

euler::graphics::Renderer::Config
euler::graphics::Renderer::config() const
{
	return from_vk2d(vk2dRendererGetConfig());
}

void
euler::graphics::Renderer::set_config(const Config &config)
{
	const auto vk2d_config = to_vk2d(config);
	vk2dRendererSetConfig(vk2d_config);
}

/* ReSharper disable once CppMemberFunctionMayBeStatic */
void
euler::graphics::Renderer::reset_swapchain()
{
	vk2dRendererResetSwapchain();
}
// bool
// Euler::Graphics::Renderer::loop(const std::function<bool(SDL_Event *)>
// &input,
//     const std::function<void()> &gui)
// {
// 	static constexpr vec4 CLEAR = { 0.0, 0.5, 1.0, 1.0 };
// 	bool quit = false;
// 	_gui->input([&]() {
// 		SDL_Event e;
// 		while (SDL_PollEvent(&e)) {
// 			if (e.type == SDL_EVENT_QUIT) quit = true;
// 			if (!input(&e)) quit = true;
// 		}
// 	});
// 	{
// 		vk2dRendererStartFrame(CLEAR);
// 		DEFER([&]() { vk2dRendererEndFrame(); });
// 		_gui->draw(gui);
// 	}
//
// 	return !quit;
// }

void
euler::graphics::Renderer::set_application(
    const std::function<bool(SDL_Event *)> &input,
    const std::function<bool()> &draw)
{
	struct LambdaApp : public Application {
		bool
		input(SDL_Event *e) override
		{
			return input_fn(e);
		}
		bool
		draw() override
		{
			return draw_fn();
		}
		const std::function<bool(SDL_Event *)> &input_fn;
		const std::function<bool()> &draw_fn;
		LambdaApp(const util::WeakReference<Renderer> &renderer,
		    const std::function<bool(SDL_Event *)> &input,
		    const std::function<bool()> &draw)
		    : Application(renderer)
		    , input_fn(input)
		    , draw_fn(draw)
		{
		}
	};
	const auto app = util::make_reference<LambdaApp>(
	    util::WeakReference(this), input, draw);
	set_application(app);
}

bool
euler::graphics::Renderer::process()
{
	static constexpr vec4 CLEAR = { 0.0, 0.5, 1.0, 1.0 };
	bool quit = false;
	_gui->input([&]() {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) quit = true;
			if (!_app->input(&e)) quit = true;
			_gui->process_event(&e);
		}
	});
	{
		vk2dRendererStartFrame(CLEAR);
		DEFER([&]() { vk2dRendererEndFrame(); });
		_gui->draw([&]() { quit = !_app->draw() || quit; });
	}

	return !quit;
}

euler::graphics::Renderer::LoggerContext::LoggerContext(
    const util::Reference<util::Logger> &logger)
{
	using LogCtxRef = util::Reference<LoggerContext>;
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
		.context = util::Reference(this).wrap(),
	};
}

euler::graphics::Renderer::LoggerContext::~LoggerContext()
{
	delete ptr;
	ptr = nullptr;
}