cmake_minimum_required(VERSION 3.20.0)
project(helloarm C CXX ASM)

set(CPU -mcpu=cortex-m3)

add_subdirectory(CMSIS)
add_subdirectory(MDR32F9Qx_StdPeriph_Driver)

set(APP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

list(APPEND INCLUDE_DIR_FOR_APP ${CMAKE_SOURCE_DIR}/CMSIS/Core/) 
list(APPEND INCLUDE_DIR_FOR_APP ${CMAKE_SOURCE_DIR}/CMSIS/Device/MDR32F9Qx/inc/) 
list(APPEND INCLUDE_DIR_FOR_APP ${CMAKE_SOURCE_DIR}/CMSIS/Device/MDR32F9Qx/startup/)
list(APPEND INCLUDE_DIR_FOR_APP ${CMAKE_SOURCE_DIR}/MDR32F9Qx_StdPeriph_Driver/inc/) 
list(APPEND INCLUDE_DIR_FOR_APP ${CMAKE_SOURCE_DIR}/include/)
list(APPEND INCLUDE_DIR_FOR_APP ${CMAKE_SOURCE_DIR}/utils/bithacks/)
list(APPEND INCLUDE_DIR_FOR_APP ${CMAKE_SOURCE_DIR}/utils/pinlist/)
list(APPEND INCLUDE_DIR_FOR_APP ${CMAKE_SOURCE_DIR}/utils/typelist/)

add_executable(${PROJECT_NAME} ${APP_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR_FOR_APP})
target_compile_options(${PROJECT_NAME} PRIVATE ${CPU})
target_link_options(${PROJECT_NAME} PRIVATE -T${CMAKE_SOURCE_DIR}/ldscripts/MDR32F9Qx.ld)
target_link_libraries(${PROJECT_NAME} $<TARGET_OBJECTS:cmsis_o> $<TARGET_OBJECTS:spd_o>)


