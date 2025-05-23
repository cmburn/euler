Anything outside fmt, mruby, Vulkan, Eigen, or SDL3 should not escape via 
headers.

Would like to hide fmt at some point, but the logger exposes it via a template
so it's tricky.
