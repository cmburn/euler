GLM, MRuby, SDL, and Vulkan are the only dependencies that should be exposed
via headers. Everything else should be hidden in the source files, even at the
cost of some duplication.

This is to ensure changes can be made for the sake of portability. Platforms
that do not support these dependencies are not expected to be supported.

# TODO order

- [X] euler::util
- [ ] euler::vulkan
- [ ] euler::graphics
- [ ] euler::game
- [ ] euler::gui
- [ ] euler::physics
