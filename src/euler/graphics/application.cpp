/* SPDX-License-Identifier: ISC */

#include "euler/graphics/application.h"
#include "euler/graphics/renderer.h"

Euler::Graphics::Application::Application(
    const Util::WeakReference<Renderer> &renderer)
	: _renderer(renderer.to_ref())
{
	_gui = _renderer->gui();
}

Euler::Util::Reference<Euler::Graphics::Renderer>
Euler::Graphics::Application::renderer() const
{
	return _renderer.to_ref();
}