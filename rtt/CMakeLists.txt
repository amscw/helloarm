set(RTT_TARGET_NAME rtt_o)

add_library(${RTT_TARGET_NAME} OBJECT 
    "SEGGER_RTT_ASM_ARMv7M.s" 
    "SEGGER_RTT_printf.c" 
    "SEGGER_RTT.c" 
)
target_include_directories(${RTT_TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(${RTT_TARGET_NAME} PRIVATE ${CPU})