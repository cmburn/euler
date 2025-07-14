/* SPDX-License-Identifier: ISC */

#include "euler/graphics/gui.h"
#include "euler/graphics/renderer.h"

#include "VK2D/nuklear_defs.h"
#include "euler/graphics/application.h"

using namespace euler;

class MainApp : public graphics::Application {
public:
	MainApp(const util::WeakReference<graphics::Renderer> &renderer)
	    : Application(renderer)
	{
	}

	bool input(SDL_Event *e) override;
	bool draw() override;
	int op = 0;
};

bool
MainApp::input(SDL_Event *)
{
	return true;
}

bool
MainApp::draw()
{
	auto ctx = _gui->context();
	nk_layout_row_dynamic(ctx, 30, 2);
	if (nk_option_label(ctx, "easy", op == 0)) op = 0;
	if (nk_option_label(ctx, "hard", op == 1)) op = 1;

	return true;
}

int
main()
{
	auto renderer = util::make_reference<graphics::Renderer>();
	auto app = util::make_reference<MainApp>(renderer.weaken());
	renderer->set_application(app);

	auto last_report = std::chrono::high_resolution_clock::now();
	double frame_count = 0;
	uintmax_t frame_total = 0;
	while (renderer->process()) {
		if (auto delta
		    = std::chrono::high_resolution_clock::now() - last_report;
		    delta >= std::chrono::seconds(1)) {
			const auto sec = std::chrono::duration<double>(delta).count();
			renderer->log()->info("FPS: {}", frame_count / sec);
			last_report = std::chrono::high_resolution_clock::now();
			frame_count = 0;
		}
		++frame_count;
		++frame_total;
	}

	renderer->log()->info("Processed {} frames", frame_total);

	return 0;
}