FROM debian:latest

COPY . /usr/src/euler
WORKDIR /usr/src/euler
RUN apt-get update && apt-get install -y \
        bison \
        build-essential \
        clang-format \
        cmake \
        curl \
        g++ \
        gcc \
        git \
        libglm-dev \
        liblz4-dev \
        libpciaccess0 \
        libpng-dev \
        libwayland-dev \
        libx11-dev \
        libx11-xcb-dev \
        libxcb-cursor-dev \
        libxcb-dri3-0 \
        libxcb-dri3-dev \
        libxcb-ewmh-dev \
        libxcb-keysyms1-dev \
        libxcb-present0 \
        libxcb-randr0-dev \
        libxcb-xinerama0 \
        libxcb-xinput0 \
        libxml2-dev \
        libxrandr-dev \
        libzstd-dev \
        ninja-build \
        ocaml-core \
        pkg-config \
        python-is-python3 \
        python3-jsonschema \
        qt6-base-dev \
        qtbase5-dev \
        ruby-dev \
        wayland-protocols \
        xz-utils && \
    curl -o /usr/local/vulkan.tar.xz https://sdk.lunarg.com/sdk/download/1.4.321.1/linux/vulkansdk-linux-x86_64-1.4.321.1.tar.xz && \
    mkdir /usr/local/vulkan &&
    tar xf /usr/local/vulkan.tar.xz -C /usr/local/vulkan --strip-components=1 \
    source /usr/local/vulkan/setup-env.sh && \
    cd /usr/src/euler/extern/mruby && \
    rake deep_clean && \
    MRUBY_CONFIG=../mruby-config.rb rake all && \
    cd /usr/src/euler/ && \
    mkdir build && \
    cd build && \
    cmake -G Ninja .. && \
    ninja -j$(nproc)
