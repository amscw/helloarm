cmake_minimum_required(VERSION 3.23.2)
project(helloarm C CXX ASM)

set (EXECUTABLE ${PROJECT_NAME})
set(CPU -mcpu=cortex-m3)
set (LD "${CMAKE_SOURCE_DIR}/ldscripts/MDR32F9Qx.ld")

add_subdirectory(CMSIS)
add_subdirectory(MDR32F9Qx_StdPeriph_Driver)
add_subdirectory(rtt)
# add_subdirectory(FreeRTOS)

add_executable(${EXECUTABLE} 
    "src/main.cpp"
    #"src/delay.c"
    #"src/params.cpp" 
    #"src/pwm_timer.cpp" 
    #"src/screen.cpp" 
    # "src/semihosting.c"
    #"src/state_indication.cpp" 
    # "src/system_exc.c" 
    #"src/tests.cpp" 
    # "src/trace.c"
    #"src/upd_timer.cpp"
    #"src/OS/hooks.c" 
    #"src/OS/os_mutex.cpp" 
    #"src/OS/os_queue.cpp" 
    #"src/OS/os_semaphore.cpp" 
    #"src/OS/os_tasks.cpp" 
    #"src/OS/os_timers.cpp" 
    #"src/OS/os.cpp"
)

target_include_directories(${EXECUTABLE} PRIVATE 
    "include/"
    "include/OS/"
    "CMSIS/Device/MDR32F9Qx/inc/"
    "CMSIS/Core/"
    "MDR32F9Qx_StdPeriph_Driver/inc/"
    "utils/bithacks/"
    "utils/pinlist/"
    "utils/typelist/"
    "rtt/"
    "FreeRTOS/Source/include/"
    "FreeRTOS/Source/portable/GCC/ARM_CM3/"
)

target_link_libraries(${EXECUTABLE} 
    $<TARGET_OBJECTS:cmsis_o> 
    $<TARGET_OBJECTS:spd_o> 
    $<TARGET_OBJECTS:rtt_o>
    #$<TARGET_OBJECTS:rtos_o>
    #$<TARGET_OBJECTS:rtosport_o>
)

target_compile_options(${EXECUTABLE} PRIVATE ${CPU})
target_compile_definitions(${EXECUTABLE} PRIVATE USE_MDR1986VE9x)
target_link_options(${EXECUTABLE} PRIVATE ${CPU} -T ${LD})

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
)

# Convert to bin file -> add conditional check?
# NB: Generating .bin files may have a negative effect when memory is split between internal and external flash memories. 
# It may generate very large files (>= 2GB) with plenty of non-used zeros.
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)
