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
        window.cpp
        window.h
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
        text.cpp
        text.h
)

add_library(euler_gui_nuklear STATIC
        nuklear_defs.h
        nuklear.h
        nuklear_sdl_vulkan.h
        nuklear_sdl_vulkan.c
        nuklear.c
)

target_link_libraries(euler_gui PUBLIC
        euler_util
        euler_graphics
)

target_compile_options(euler_gui PRIVATE ${EULER_COMPILE_OPTIONS})

target_link_libraries(euler_gui PRIVATE
        euler_gui_nuklear
)