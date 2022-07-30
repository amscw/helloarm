set(SPD_TARGET_NAME spd_o)

add_library(${SPD_TARGET_NAME} OBJECT 
    #"src/MDR32F9Qx_adc.c" 
    #"src/MDR32F9Qx_arinc429R.c"
    #"src/MDR32F9Qx_arinc429t.c"
    #"src/MDR32F9Qx_audio.c"
    #"src/MDR32F9Qx_bkp.c"
    #"src/MDR32F9Qx_can.c"
    #"src/MDR32F9Qx_comp.c"
    #"src/MDR32F9Qx_dac.c"
    "src/MDR32F9Qx_dma.c"
    #"src/MDR32F9Qx_ebc.c"
    "src/MDR32F9Qx_eeprom.c"
    #"src/MDR32F9Qx_eth.c"
    "src/MDR32F9Qx_i2c.c"
    #"src/MDR32F9Qx_iwdg.c"
    #"src/MDR32F9Qx_keypad.c"
    #"src/MDR32F9Qx_led.c"
    #"src/MDR32F9Qx_mil_std_1553.c"
    "src/MDR32F9Qx_port.c"
    "src/MDR32F9Qx_power.c"
    "src/MDR32F9Qx_rst_clk.c"
    #"src/MDR32F9Qx_ssp.c"
    "src/MDR32F9Qx_timer.c"
    #"src/MDR32F9Qx_uart.c"
    #"src/MDR32F9Qx_usb.c"
    #"src/MDR32F9Qx_wwdg.c"
)

target_include_directories(${SPD_TARGET_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/" 
    "${CMAKE_SOURCE_DIR}/CMSIS/Core/" 
    "${CMAKE_SOURCE_DIR}/CMSIS/Device/MDR32F9Qx/inc/" 
    "${CMAKE_SOURCE_DIR}/CMSIS/Device/MDR32F9Qx/startup/"
    "${CMAKE_SOURCE_DIR}/include/"
)

target_compile_options(${SPD_TARGET_NAME} PRIVATE ${CPU})
target_compile_definitions(${SPD_TARGET_NAME} PRIVATE -DUSE_MDR1986VE9x)