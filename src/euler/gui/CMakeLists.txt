add_library(euler_gui STATIC
        button.cpp
        button.h
        checkbox.cpp
        checkbox.h
        color_picker.cpp
        color_picker.h
        dropdown.cpp
        dropdown.h
        element.cpp
        element.h
        gui.cpp
        gui.h
        knob.cpp
        knob.h
        progress_bar.cpp
        progress_bar.h
        radio_button.cpp
        radio_button.h
        row.cpp
        row.h
        selectable.cpp
        selectable.h
        slider.cpp
        slider.h
        style.cpp
        style.h
        widget.cpp
        widget.h
        window.cpp
        window.h
        text.cpp
        text.h
)

add_library(nuklear STATIC
        nuklear_shim.h
        nuklear.h
        nuklear_sdl_vulkan.h
        nuklear_shim.c
)

target_link_libraries(euler_gui PUBLIC
        euler_util
        euler_graphics
)

target_compile_options(euler_gui PRIVATE ${EULER_COMPILE_OPTIONS})

target_link_libraries(euler_gui PRIVATE
        nuklear
)