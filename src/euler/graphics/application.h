/* SPDX-License-Identifier: ISC */

#ifndef EULER_RENDERER_APPLICATION_H
#define EULER_RENDERER_APPLICATION_H

#include "SDL3/SDL_events.h"
#include "euler/util/object.h"
#include "gui.h"

namespace Euler::Graphics {
class Renderer;

class Application : public Util::Object {
public:
	Application(const Util::WeakReference<Renderer> &);
	~Application() override = default;
	virtual bool input(SDL_Event *e) = 0;
	virtual bool draw() = 0;
	Util::Reference<Renderer> renderer() const;

	Util::Reference<Gui>
	gui() const
	{
		return _gui;
	}

protected:
	Util::WeakReference<Renderer> _renderer;
	Util::Reference<Gui> _gui;
};
} /* namespace Euler::Graphics */

#endif /* EULER_RENDERER_APPLICATION_H */
