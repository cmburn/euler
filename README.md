Anything outside mruby, Vulkan, Eigen, or SDL3 should not escape via headers.


Note that the physics API and the rendering-specific portions of the graphics
API are likely to change! I intend to keep the GUI elements of the graphics
module the same, although I may split these into a separate gui module.

