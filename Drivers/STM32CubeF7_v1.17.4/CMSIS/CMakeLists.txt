cmake_minimum_required(VERSION 3.15)

add_library(cmsis STATIC
        Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c
)

target_include_directories(cmsis PUBLIC
        Core/Include
        Device/ST/STM32F7xx/Include
        ${CMAKE_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc
        ${CMAKE_SOURCE_DIR}/Src
)

target_compile_definitions(cmsis PUBLIC
        -DSTM32F746xx
        # -DUSE_HAL_DRIVER
        # -DUSE_FULL_ASSERT
)

# target_link_options(cmsis PUBLIC -T${STM32_LINKER_SCRIPT})
