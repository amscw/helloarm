set(CMSIS_TARGET_NAME cmsis_o)

add_library(${CMSIS_TARGET_NAME} OBJECT 
    "Device/MDR32F9Qx/startup/startup_MDR32F9Qx.s" 
    "Device/MDR32F9Qx/startup/system_MDR32F9Qx.c"
)

target_include_directories(${CMSIS_TARGET_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/" # core_cm3.h
    "${CMAKE_CURRENT_SOURCE_DIR}/Device/MDR32F9Qx/startup/" # system_MDR32F9Qx.h
    "${CMAKE_CURRENT_SOURCE_DIR}/Device/MDR32F9Qx/inc/" # MDR32Fx.h
    "${CMAKE_SOURCE_DIR}/include/" # MDR32F9Qx_board.h, MDR32F9Qx_config.h
    "${CMAKE_SOURCE_DIR}/MDR32F9Qx_StdPeriph_Driver/inc/" # MDR32F9Qx_lib.h
)

target_compile_options(${CMSIS_TARGET_NAME} PRIVATE ${CPU})
target_compile_definitions(${CMSIS_TARGET_NAME} PRIVATE -DUSE_MDR1986VE9x)