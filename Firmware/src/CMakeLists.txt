set(PROJECT kit_controller)

# Just the version i was using at the time
cmake_minimum_required(VERSION 3.30)

# pico_sdk_import
include(../pico_sdk_import.cmake)

project(${PROJECT} C CXX)
pico_sdk_init()

add_executable(${ProjectName}
    main.c
    console.c
    command_dispatcher.c
    usb_console.c
    parser.c
    led_task.c
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${ProjectName} 
    pico_stdlib                # Obviously we need this one
    FreeRTOS-Kernel-Heap4      # From FreeRTOS
    cmake_git_version_tracking # https://github.com/andrew-hardin/cmake-git-version-tracking Very handy library!
    )

# Setting these will configure USB for serial output and disable uart
pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})
