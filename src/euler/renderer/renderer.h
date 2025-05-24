/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_RENDERER_H
#define EULER_RENDERER_RENDERER_H

#include "Eigen/Core"

#include "euler/renderer/application.h"
#include "euler/renderer/gui.h"
#include "euler/renderer/window.h"

#include "euler/util/object.h"
#include "euler/util/version.h"

typedef struct VK2DRenderer_t *VK2DRenderer;
typedef struct VK2DLogger VK2DLogger;

namespace Euler::Renderer {
class Renderer final : public Util::Object {
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
	Renderer(const Util::Reference<Util::Logger> &log
	    = Util::make_reference<Util::Logger>("Renderer"),
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
	void set_application(const Util::Reference<Application> &app) {
		_app = app;
	}
	void set_application(const std::function<bool(SDL_Event *)> &input,
	    const std::function<bool()> &gui);

	[[nodiscard]] Util::Reference<Window>
	window() const
	{
		return _window;
	}

	[[nodiscard]] Util::Reference<Application>
	application() const
	{
		return _app;
	}

	[[nodiscard]] Util::Reference<Gui>
	gui() const
	{
		return _gui;
	}

	[[nodiscard]] Util::Reference<Util::Logger>
	log() const
	{
		return _log;
	}

private:
	struct LoggerContext final : Object {
		LoggerContext(const Util::Reference<Util::Logger> &logger);
		~LoggerContext() override;
		Util::Reference<Util::Logger> log;
		VK2DLogger *ptr = nullptr;
	};

	VK2DRenderer _renderer;
	Util::Reference<Window> _window;
	Util::Reference<Gui> _gui;
	Util::Reference<Util::Logger> _log;
	Config _config;
	Util::Reference<LoggerContext> _logger_context;
	Util::Reference<Application> _app;
};
} /* namespace Euler::Renderer */

#endif /* EULER_RENDERER_RENDERER_H */
