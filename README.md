# Notes
This is very much a WIP, and nothing works whatsoever.

The docker file isn't functional currently, but it still gives a decent idea
of the dependencies required/build process.

GLM, MRuby, SDL, and Vulkan are the only dependencies that should be exposed
via headers. Everything else should be hidden in the source files, even at the
cost of some duplication.

This is to ensure changes can be made for the sake of portability. Platforms
that do not support these dependencies are not expected to be supported.

Only PC and Switch are targets at the moment.

A minimum of three threads is required. One for SDL, one for Vulkan, and one
for the mRuby interpreter.

The engine should be fully thread-safe, anything else is a bug.

Each additional game state will require a thread of its own.

# Install Instructions

## Dependencies

CMake, Ruby, Vulkan SDK 1.4+, and a C++20 compiler are required.

### Linux

It is recommended to [follow the Vulkan SDK installation instructions](
    https://vulkan.lunarg.com/doc/view/latest/linux/getting_started.html
) to install the SDK. The bin directory of the SDK must be in your PATH.
On Linux, the easiest way to ensure this is by running 
`source <path-to-sdk>/setup-env.sh`. It's easiest to add this to your
.bashrc or equivalent.

Currently, the building for mRuby is not integrated into the CMake build. To
build it, change into the `extern/mruby` directory and run 
`MRUBY_CONFIG=../mruby-config.rb rake all`.

Once this is done, you can perform the normal CMake build process, return to
the root directory, make a `build` directory, change into it, and run
`cmake ..` followed by `cmake --build .`.

The file in `bin/main.rb` is the sample entry point for the engine. You can run
it after building with `euler bin/main.rb` from the root source directory.
