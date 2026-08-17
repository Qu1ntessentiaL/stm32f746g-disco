# STM32F746 Firmware Project
cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project Configuration
project(stm32f746g-disco
        VERSION 1.0.0
        LANGUAGES C CXX ASM
)

# C/C++ Standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif ()

# Disable Link Time Optimization for reduced binary size
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)

# CMake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

########################################################

# Enable Link Time Optimization for reduced binary size
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
string(APPEND CMAKE_C_FLAGS_RELEASE " -flto=auto")
string(APPEND CMAKE_CXX_FLAGS_RELEASE " -flto=auto")

add_subdirectory(Drivers/CMSIS)
add_subdirectory(Drivers/STM32F7xx_HAL_Driver)
add_subdirectory(Drivers/BSP)
add_subdirectory(Drivers/FatFs)

# LV_BUILD_CONF_PATH необходим для версии v9.3
# set(LV_BUILD_CONF_PATH "${CMAKE_SOURCE_DIR}/Src/lv_conf.h" CACHE STRING "" FORCE)
# Add the lv_conf.h path as a compiler definition so LVGL can find it during compilation
# Note: No quotes - let __LV_TO_STR() stringify the path to avoid escaped quotes in the macro
add_compile_definitions(LV_CONF_PATH=${CMAKE_SOURCE_DIR}/Src/lv_conf.h)
add_subdirectory(Drivers/lvgl)

set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(NO_STL ON CACHE BOOL "" FORCE)   # Важно для embedded!
# Подавляю warning,связанный с include(GNUInstallDirs) в embedded проекте
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "")
add_subdirectory(Drivers/etl EXCLUDE_FROM_ALL)
unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)

add_executable(${PROJECT_NAME}
        ${STARTUP_FILE}
)

include(Src/app-sources.cmake)
include(eez-ui/eez-sources.cmake)

# ---------------------------------------------------------------------------------------------------------------------
set(FREERTOS_KERNEL_PATH "Drivers/FreeRTOS-Kernel")

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config INTERFACE
        "Src/FreeRTOS"
)

if (DEFINED FREERTOS_SMP_EXAMPLE AND FREERTOS_SMP_EXAMPLE STREQUAL "1")
    message(STATUS "Build FreeRTOS SMP example")
    # Adding the following configurations to build SMP template port
    add_compile_options(-DconfigNUMBER_OF_CORES=2 -DconfigUSE_PASSIVE_IDLE_HOOK=0)
endif ()

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

set(FREERTOS_PORT "GCC_ARM_CM7" CACHE STRING "" FORCE)

add_subdirectory(${FREERTOS_KERNEL_PATH} FreeRTOS-Kernel)

# ---------------------------------------------------------------------------------------------------------------------

target_link_libraries(lvgl PUBLIC fatfs)

target_link_libraries(${PROJECT_NAME} PUBLIC
        bsp
        freertos_kernel
        fatfs
        lvgl
        etl::etl
)

# ===================
# Post-Build Commands
# ===================
include(PostBuild)

# Add post-build commands (HEX, BIN, disassembly)
add_firmware_post_build_commands(${PROJECT_NAME})

# Add analysis targets
add_symbol_sizes_postbuild(${PROJECT_NAME})
add_symbol_addresses_postbuild(${PROJECT_NAME})
add_text_info_target(${PROJECT_NAME})

# Add flash operations
add_erase_flash_target()
add_flash_target(${PROJECT_NAME})

# =============
# Build Summary
# =============
message(STATUS "")
message(STATUS "======= Build Configuration =======")
message(STATUS "Project:      ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Generator:    ${CMAKE_GENERATOR}")
message(STATUS "Binary dir:   ${CMAKE_BINARY_DIR}")
message(STATUS "Build Type:   ${CMAKE_BUILD_TYPE}")
message(STATUS "Toolchain:    ${CMAKE_C_COMPILER_ID}")
message(STATUS "C Standard:   ${CMAKE_C_STANDARD}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "===================================")
message(STATUS "")