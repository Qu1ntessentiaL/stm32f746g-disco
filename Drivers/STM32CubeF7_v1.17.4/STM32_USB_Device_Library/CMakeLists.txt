cmake_minimum_required(VERSION 3.15)

set(LIBRARY_NAME usb_device_msc)

# Найти все C-файлы в директории
file(GLOB_RECURSE ALL_C_SOURCES
        Core/Src/*.c
        Class/MSC/Src/*.c
)

# Исключить файлы по шаблону
list(FILTER ALL_C_SOURCES EXCLUDE REGEX ".*usbd_conf_template\\.c$")
list(FILTER ALL_C_SOURCES EXCLUDE REGEX ".*usbd_desc_template\\.c$")
list(FILTER ALL_C_SOURCES EXCLUDE REGEX ".*usbd_msc_storage_template\\.c$")


add_library(${LIBRARY_NAME} STATIC
        ${ALL_C_SOURCES}
)

target_compile_definitions(${LIBRARY_NAME} PUBLIC
        -DUSE_USB_FS
)

target_include_directories(${LIBRARY_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/Class/MSC/Inc
        ${CMAKE_CURRENT_LIST_DIR}/Core/Inc
)

target_link_libraries(${LIBRARY_NAME} PUBLIC bsp)