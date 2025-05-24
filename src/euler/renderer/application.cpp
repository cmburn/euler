/* SPDX-License-Identifier: ISC */

#include "euler/renderer/application.h"
#include "euler/renderer/renderer.h"

Euler::Renderer::Application::Application(
    const Util::WeakReference<Renderer> &renderer)
	: _renderer(renderer.to_ref())
{
	_gui = _renderer->gui();
}

Euler::Util::Reference<Euler::Renderer::Renderer>
Euler::Renderer::Application::renderer() const
{
	return _renderer.to_ref();
}