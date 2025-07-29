add_library(euler_gui STATIC
        button.cpp
        button.h
        checkbox.cpp
        checkbox.h
        color_picker.cpp
        color_picker.h
        element.cpp
        element.h
        knob.cpp
        knob.h
        progress_bar.cpp
        progress_bar.h
        radio.cpp
        radio.h
        selectable.cpp
        selectable.h
        slider.cpp
        slider.h
        symbol.cpp
        symbol.h
        widget.cpp
        widget.h
        ext.cpp
        ext.h
)

target_link_libraries(euler_gui PUBLIC
        euler_util
        euler_graphics
)