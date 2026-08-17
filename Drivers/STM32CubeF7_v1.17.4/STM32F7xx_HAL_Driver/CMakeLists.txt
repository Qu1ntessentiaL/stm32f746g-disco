cmake_minimum_required(VERSION 3.15)

set(HAL_CONF_H_DIR ${CMAKE_SOURCE_DIR}/Src)

#Найти все C-файлы в директории
file(GLOB_RECURSE ALL_C_SOURCES "Src/*.c")

# Исключить некоторые файлы
list(REMOVE_ITEM ALL_C_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32f7xx_hal_msp_template.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32f7xx_hal_timebase_rtc_alarm_template.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32f7xx_hal_timebase_rtc_wakeup_template.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Src/stm32f7xx_hal_timebase_tim_template.c
)

add_library(hal STATIC
        ${ALL_C_SOURCES}
)

target_include_directories(hal PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc/
        # ${CMAKE_CURRENT_SOURCE_DIR}/Inc/Legacy/
        ${HAL_CONF_H_DIR}
)

target_link_libraries(hal PUBLIC cmsis)
