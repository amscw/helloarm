cmake_minimum_required(VERSION 3.20.0)
project(helloarm)

set(CPU -mcpu=cortex-m3)

add_subdirectory(CMSIS)
add_subdirectory(MDR32F9Qx_StdPeriph_Driver)
#add_subdirectory(src)