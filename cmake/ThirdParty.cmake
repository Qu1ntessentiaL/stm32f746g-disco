# ===================================
# Third-Party Libraries Configuration
# ===================================

# -------------------
# STM32 HAL and CMSIS
# -------------------
function(add_stm32_hal)
    set(STM32_CUBE_F7_DIR ${CMAKE_SOURCE_DIR}/Drivers/STM32CubeF7_v1.17.4)

    # Keep project-owned HAL config in Drivers/config/HAL.
    # We provide:
    # 1) one-time seeding from CubeMX (if dst doesn't exist),
    # 2) a manual sync target to refresh it on demand after CubeMX regen.
    set(HAL_CONF_SRC ${CMAKE_SOURCE_DIR}/CoGen/Core/Inc/stm32f7xx_hal_conf.h)
    set(HAL_CONF_DST_DIR ${CMAKE_SOURCE_DIR}/Src)
    set(HAL_CONF_DST ${HAL_CONF_DST_DIR}/stm32f7xx_hal_conf.h)
    if (EXISTS ${HAL_CONF_SRC} AND NOT EXISTS ${HAL_CONF_DST})
        file(MAKE_DIRECTORY ${HAL_CONF_DST_DIR})
        configure_file(${HAL_CONF_SRC} ${HAL_CONF_DST} COPYONLY)
        message(STATUS "HAL config: seeded ${HAL_CONF_DST} from CubeMX (${HAL_CONF_SRC})")
    endif ()

    if (EXISTS ${HAL_CONF_SRC} AND NOT TARGET sync_hal_conf)
        add_custom_target(sync_hal_conf
            COMMAND ${CMAKE_COMMAND} -E make_directory ${HAL_CONF_DST_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${HAL_CONF_SRC} ${HAL_CONF_DST}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Sync HAL config: cube/Core/Inc/stm32h7xx_hal_conf.h -> third_party/config/HAL/stm32h7xx_hal_conf.h"
            VERBATIM
        )
    endif ()

    add_subdirectory(${STM32_CUBE_F7_DIR}/CMSIS)
    add_subdirectory(${STM32_CUBE_F7_DIR}/STM32F7xx_HAL_Driver)
    # add_subdirectory(${STM32_CUBE_F7_DIR}/Utilities/Fonts)

    message(STATUS "STM32 HAL: Configured from ${STM32_CUBE_F7_DIR}")
endfunction()

# -------------------------------------------
# LVGL (Light and Versatile Graphics Library)
# -------------------------------------------
function(add_lvgl_library)
    # Do not create the lvgl_examples/lvgl_demos/lvgl_thorvg targets.
    # These names are read by lvgl/env_support/cmake/custom.cmake;
    # the CONFIG_LV_* ones only work in the ESP-IDF/Kconfig build.
    set(LV_CONF_BUILD_DISABLE_EXAMPLES ON CACHE BOOL "" FORCE)
    set(LV_CONF_BUILD_DISABLE_DEMOS ON CACHE BOOL "" FORCE)
    set(LV_CONF_BUILD_DISABLE_THORVG_INTERNAL ON CACHE BOOL "" FORCE)

    # Set LVGL configuration file path
    add_compile_definitions(LV_CONF_PATH=${CMAKE_SOURCE_DIR}/Src/lv_conf.h)
    # set(LV_BUILD_CONF_PATH
    #         ${CMAKE_SOURCE_DIR}/Drivers/config/lvgl/lv_conf.h
    #         CACHE STRING "" FORCE
    # )
    add_subdirectory(${CMAKE_SOURCE_DIR}/Drivers/lvgl)
    # target_link_libraries(lvgl
    #     PRIVATE
    #         fatfs
    # )

    message(STATUS "LVGL: Configured with ${LV_CONF_PATH}")
endfunction()

# --------
# FreeRTOS
# --------
function(add_freertos_library)
    set(FREERTOS_KERNEL_PATH ${CMAKE_SOURCE_DIR}/Drivers/FreeRTOS-Kernel)

    # Create FreeRTOS config interface library
    add_library(freertos_config INTERFACE)
    target_include_directories(freertos_config
        INTERFACE
            ${CMAKE_SOURCE_DIR}/Src/FreeRTOS
    )

    # Configure FreeRTOS
    set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
    set(FREERTOS_PORT "GCC_ARM_CM7" CACHE STRING "" FORCE)

    # Add FreeRTOS subdirectory
    add_subdirectory(${FREERTOS_KERNEL_PATH})

    # Link config to kernel
    target_link_libraries(freertos_kernel
        PRIVATE
            freertos_config
    )

    message(STATUS "FreeRTOS: Configured with heap_${FREERTOS_HEAP} and ${FREERTOS_PORT} port")
endfunction()

# ------------------------
# wsh-shell (Whoosh Shell)
# ------------------------
function(add_wsh_shell_library)
    add_subdirectory(${CMAKE_SOURCE_DIR}/shell-logger)

    message(STATUS "wsh-shell: Configured")
endfunction()

# ------------------------------------------
# ETL (Embedded Template Library) - Optional
# ------------------------------------------
function(add_etl_library)
    set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(NO_STL ON CACHE BOOL "" FORCE)

    # Suppress GNUInstallDirs warnings for embedded projects
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "")
    add_subdirectory(Drivers/etl EXCLUDE_FROM_ALL)
    unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)

    message(STATUS "ETL: Configured (NO_STL mode)")
endfunction()

# -------------
# FatFS Library
# -------------
function(add_fatfs_library)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Drivers/STM32CubeF7_v1.17.4/FatFs)

    message(STATUS "FatFs configured")
endfunction()

# --------------
# USB device MSC
# --------------
function(add_usbd_msc_library)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Drivers/STM32CubeF7_v1.17.4/STM32_USB_Device_Library)

    message(STATUS "USB device MSC configured")
endfunction()

# -----------------------------------
# Configure All Third-Party Libraries
# -----------------------------------
macro(configure_all_third_party_libraries)
    message(STATUS "===== Configuring Third-Party Libraries =====")

    add_stm32_hal()
    add_lvgl_library()
    add_freertos_library()
    # add_wsh_shell_library()
    add_etl_library()
    add_fatfs_library()
    # add_usbd_msc_library()

    message(STATUS "===== Third-Party Libraries Configured =====")
endmacro()
