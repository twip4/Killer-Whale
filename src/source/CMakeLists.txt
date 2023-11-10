aux_source_directory(. DIR_source_SRCS)

include_directories(../lib/Config)
include_directories(../lib/GUI)
include_directories(../lib/LCD)
include_directories(../lib/QMI8658)
include_directories(../lib/Touch)

add_library(source ${DIR_source_SRCS})
target_link_libraries(source PUBLIC Config LCD Touch QMI8658 hardware_clocks hardware_rtc hardware_adc)