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

# ===================
# Memory layout alias
# ===================
# add_subdirectory(toolchain/mem_sections)

# =====================
# Third-Party Libraries
# =====================
include(ThirdParty)
configure_all_third_party_libraries()

# ===============
# Main Executable
# ===============

# Startup and system files
set(STARTUP_FILE ${CMAKE_SOURCE_DIR}/toolchain/startup_stm32f746xx.s)
set(SYSTEM_CFG ${CMAKE_SOURCE_DIR}/toolchain/system_stm32h7xx.c)

# Create executable
add_executable(${PROJECT_NAME}
        ${STARTUP_FILE}
        # ${SYSTEM_CFG}
)

# ==================
# Project Components
# ==================

# CubeMX generated files
# add_subdirectory(cube/Core)

# BSP layer code
add_subdirectory(Drivers/STM32CubeF7_v1.17.4/BSP)

# Application sources
# add_subdirectory(src)
include(Src/app-sources.cmake)

# GUI sources
# add_subdirectory(eez-ui)
include(eez-ui/eez-sources.cmake)

# ==============
# Link Libraries
# ==============
target_link_libraries(lvgl PUBLIC fatfs) # Костыль
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        bsp
        freertos_kernel
        lvgl
        etl::etl
    PRIVATE
        # mem_layout
)

# Optional: ETL support
if (NO_STL)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ETL_NO_STL)
endif ()

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