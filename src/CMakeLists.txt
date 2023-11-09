cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(KILLER_WHALE)

pico_sdk_init()

add_subdirectory(./lib/Config)
add_subdirectory(./lib/Fonts)
add_subdirectory(./lib/GUI)
add_subdirectory(./lib/LCD)
add_subdirectory(./lib/Touch)
add_subdirectory(./lib/QMI8658)
add_subdirectory(./examples)

include_directories(./examples)
include_directories(./lib/Config)
include_directories(./lib/Fonts)
include_directories(./lib/GUI)
include_directories(./lib/LCD)
include_directories(./lib/Touch)
include_directories(./lib/QMI8658)

add_executable(src main.c)

pico_enable_stdio_usb(src 1)
pico_enable_stdio_uart(src 0)

pico_add_extra_outputs(src)

target_link_libraries(src examples LCD Touch QMI8658 GUI Fonts Config pico_stdlib hardware_spi hardware_i2c)