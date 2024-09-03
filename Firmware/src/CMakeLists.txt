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
    can_controller.c
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../lib/pico-mcp2515/include # MCP_CAN library
)

target_link_libraries(${ProjectName} 
    pico_stdlib                # Obviously we need this one
    hardware_spi               # SPI library pico
    FreeRTOS-Kernel-Heap4      # From FreeRTOS
    pcio-mcp2515               # MCP_CAN library
    cmake_git_version_tracking # https://github.com/andrew-hardin/cmake-git-version-tracking Very handy library!
    )

# Setting these will configure USB for serial output and disable uart
pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})
