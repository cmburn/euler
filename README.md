# Notes
This is very much a WIP, and nothing works whatsoever.

The docker file isn't functional currently, but it still gives a decent idea
of the dependencies required/build process.

GLM, MRuby, SDL, and Vulkan are the only dependencies that should be exposed
via headers. Everything else should be hidden in the source files, even at the
cost of some duplication. This is to ensure changes can be made for the sake 
of portability. Platforms that do not support these dependencies are not 
expected to be supported.

The engine should be fully thread-safe, anything else is a bug.


The general goals for Euler is to create an engine focused on:
- Vulkan-based graphics
- Portability across PC platforms (Windows, Mac, Linux, *BSD and ultimately
  Nintendo Switch)
- Tailored for 2D platformers
- An emphasis on physics simulation


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

## Common Build Issues

* If the build fails due to a missing Vulkan version, ensure that the
  `VULKAN_SDK` environment variable is set to the root of your Vulkan SDK
  installation, as CMake will sometimes try to use the system's Vulkan
  instead.
* If the shader compilation fails due to a missing `glslc` or `slangc` 
  executable, ensure that the `bin` directory of your Vulkan SDK is passed as
  a part of your `PATH` environment variable for CMake, as CMake likes to 
  ignore whatever you set for it in a normal shell.
* If the bill fails due to being unable to download a SPIR-V dependency, you 
  may need to set your `LD_LIBRARY_PATH` to include the `lib` directory of your
  Vulkan SDK installation, as once again CMake likes to ignore whatever you set
  for it in a normal shell.

# Miscellaneous

This is not intended to be a general-purpose engine. It's been developed for
a particular project of mine, and tradeoffs have been made to suit that 
project's needs. Pull requests are welcome, but may be rejected if they
impede the usage I'm aiming for.

If you're looking for a general-purpose Ruby engine, I recommend [DragonRuby](
https://dragonruby.org/
), which has a wonderful community and is very well-maintained. [Love2D](
https://love2d.org/
) is also fantastic, and much of Euler's API has been patterned after it.
