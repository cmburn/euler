/* SPDX-License-Identifier: ISC */

#include "euler/renderer/gui.h"
#include "euler/renderer/renderer.h"

#include "VK2D/nuklear_defs.h"
#include "euler/renderer/application.h"

using namespace Euler;

class MainApp : public Renderer::Application {
public:
	MainApp(const Util::WeakReference<Renderer::Renderer> &renderer)
	    : Application(renderer)
	{
	}

	bool input(SDL_Event *e) override;
	bool draw() override;
};

bool
MainApp::input(SDL_Event *)
{
	return true;
}

bool
MainApp::draw()
{
	return true;
}

int
main()
{
	auto renderer = Util::make_reference<Renderer::Renderer>();
	auto app = Util::make_reference<MainApp>(renderer.weaken());
	renderer->set_application(app);

	// auto last_time = std::chrono::high_resolution_clock::now();
	// while (renderer->process()) {
	//     auto current_time = std::chrono::high_resolution_clock::now();
	//     std::chrono::duration<double> elapsed = current_time - last_time;
	//     printf("Elapsed time: %.6f seconds\n", elapsed.count());
	//     last_time = current_time;
	// }

	auto last_report = std::chrono::high_resolution_clock::now();
	double frame_count = 0;
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
	}

	return 0;
}