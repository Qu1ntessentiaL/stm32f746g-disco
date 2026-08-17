cmake_minimum_required(VERSION 3.15)

set(LIBRARY_NAME bsp)
# set(BSP_CONF_H_DIR ${CMAKE_SOURCE_DIR}/Src)

file(GLOB_RECURSE SRC_DISCO
        ${CMAKE_CURRENT_LIST_DIR}/STM32746G-Discovery/*.c
)

add_library(${LIBRARY_NAME} STATIC
        Components/ft5336/ft5336.c
        Components/lan8742/lan8742.c

        ${SRC_DISCO}
)

target_include_directories(${LIBRARY_NAME} PUBLIC
        Components/Common
        Components/rk043fn48h
        Components/ft5336
        Components/lan8742
        Components/w25q128j
        STM32746G-Discovery
        # ${BSP_CONF_H_DIR}
)

target_compile_definitions(${LIBRARY_NAME} PUBLIC
        -DUSE_STM32746G_DISCO_REVC03
)

target_link_libraries(${LIBRARY_NAME} PUBLIC hal)