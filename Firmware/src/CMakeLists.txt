add_executable(${ProjectName}
    main.cpp
    usb_console.cpp
    parser.cpp
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
