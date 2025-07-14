/* SPDX-License-Identifier: ISC OR Apache-2.0 WITH LLVM-exception */

#ifndef EULER_RENDER_RENDERER_H
#define EULER_RENDER_RENDERER_H

namespace euler::render {
class Renderer final : public util::Object {
public:
	~Renderer() override;
	enum class MSAA {
		X1,
		X2,
		X4,
		X8,
		X16,
		X32,
	};

	enum class ScreenMode {
		Immediate,
		VSync,
		TripleBuffer,
	};

	enum class FilterType {
		Linear,
		Nearest,
	};

	struct Config {
		/* TODO: ask Paolo, `Validation: vkCreateSampler(): pCreateInfo
		 *       anisotropyEnable and unnormalizedCoordinates are both
		 *       VK_TRUE` when msaa != 1 */
		MSAA msaa = MSAA::X1;
		ScreenMode screen_mode = ScreenMode::VSync;
		FilterType filter_type = FilterType::Linear;
	};

	static Config default_config();
	Renderer(const util::Reference<util::Logger> &log
	    = util::make_reference<util::Logger>("Renderer"),
	    const Config &config = default_config(),
	    std::vector<Font> &&fonts = {});

	struct VRAMUsage {
		float in_use;
		float total;
	};
	Config config() const;
	void set_config(const Config &config);
	void reset_swapchain();
	// bool process(const std::function<bool(SDL_Event *)> &input,
	//     const std::function<bool()> &gui);
	bool process();
	void set_application(const util::Reference<Application> &app) {
		_app = app;
	}
	void set_application(const std::function<bool(SDL_Event *)> &input,
	    const std::function<bool()> &gui);

	[[nodiscard]] util::Reference<Window>
	window() const
	{
		return _window;
	}

	[[nodiscard]] util::Reference<Application>
	application() const
	{
		return _app;
	}

	[[nodiscard]] util::Reference<Gui>
	gui() const
	{
		return _gui;
	}

	[[nodiscard]] util::Reference<util::Logger>
	log() const
	{
		return _log;
	}

private:
	struct LoggerContext final : Object {
		LoggerContext(const util::Reference<util::Logger> &logger);
		~LoggerContext() override;
		util::Reference<util::Logger> log;
		VK2DLogger *ptr = nullptr;
	};

	VK2DRenderer _renderer;
	util::Reference<Window> _window;
	util::Reference<Gui> _gui;
	util::Reference<util::Logger> _log;
	Config _config;
	util::Reference<LoggerContext> _logger_context;
	util::Reference<Application> _app;
};
} /* namespace euler::render */


#endif /* EULER_RENDER_RENDERER_H */

