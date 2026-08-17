cmake_minimum_required(VERSION 3.15)

set(LIBRARY_NAME fatfs)

add_library(${LIBRARY_NAME} STATIC
        src/ff.c
        src/ff_gen_drv.c
        src/diskio.c
        src/option/syscall.c
        src/option/unicode.c
        src/customDrivers/sd_diskio_dma.c
)

target_include_directories(${LIBRARY_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/customDrivers/
)

target_compile_definitions(${LIBRARY_NAME} PUBLIC
        # Определении компилятору
)

target_link_libraries(${LIBRARY_NAME} PUBLIC
        bsp
        freertos_kernel
)
