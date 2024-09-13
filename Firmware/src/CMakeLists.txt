set(PROJECT kit_controller)

# Just the version i was using at the time
cmake_minimum_required(VERSION 3.30)

# pico_sdk_import
include(../pico_sdk_import.cmake)

project(${PROJECT} C CXX)
pico_sdk_init()

# We have to do this manually with the pico-mcp2515 library for.. some reason :/
add_subdirectory(../lib/pico-mcp2515 ${CMAKE_BINARY_DIR}/pico-mcp2515)

add_executable(${ProjectName}
    main.cpp
    console.cpp
    command_dispatcher.cpp
    usb_console.cpp
    parser.cpp
    led_task.cpp
    can_controller.cpp
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${ProjectName} 
    pico_stdlib                # Obviously we need this one
    hardware_spi               # SPI library pico
    FreeRTOS-Kernel-Heap4      # From FreeRTOS
    pico_mcp2515               # MCP_CAN library
    cmake_git_version_tracking # https://github.com/andrew-hardin/cmake-git-version-tracking Very handy library!
)


# Setting these will configure USB for serial output and disable uart
pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})
