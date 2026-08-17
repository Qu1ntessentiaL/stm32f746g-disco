# Post-Build Commands for STM32 Firmware

# Find required utilities
find_program(OBJCOPY arm-none-eabi-objcopy REQUIRED)
find_program(SIZE arm-none-eabi-size REQUIRED)
find_program(OBJDUMP arm-none-eabi-objdump REQUIRED)
find_program(NM arm-none-eabi-nm REQUIRED)

# Function to add post-build commands
function(add_firmware_post_build_commands TARGET_NAME)

    # Generate HEX and BIN files
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Generating HEX and BIN files..."

        # HEX file
        COMMAND ${OBJCOPY} -O ihex
            $<TARGET_FILE:${TARGET_NAME}>
            $<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.hex

        # BIN file
        COMMAND ${OBJCOPY} -O binary
            $<TARGET_FILE:${TARGET_NAME}>
            $<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.bin

        # Show size information
        COMMAND ${SIZE} -A -x $<TARGET_FILE:${TARGET_NAME}>

        # Generate disassembly
        COMMAND ${OBJDUMP} -d -S -M force-thumb -M reg-names-std
            $<TARGET_FILE:${TARGET_NAME}>
            > $<TARGET_FILE_BASE_NAME:${TARGET_NAME}>_disasm.s

        COMMENT "Post-build: Generating HEX, BIN, and disassembly files"
        VERBATIM
    )

endfunction()

# Custom Targets for Analysis

# ========================
# Firmware Analysis Module
# ========================

set(ANALYSIS_DIR "${CMAKE_BINARY_DIR}")

file(MAKE_DIRECTORY ${ANALYSIS_DIR})

# -----------------------
# Common header generator
# -----------------------
function(fw_write_header OUT_FILE TITLE TARGET_NAME EXTRA_INFO)

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E echo "=================================================" > ${OUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E echo "${TITLE}" >> ${OUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E echo "Target: ${TARGET_NAME}" >> ${OUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E echo "Build type: ${CMAKE_BUILD_TYPE}" >> ${OUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E echo "ELF: $<TARGET_FILE:${TARGET_NAME}>" >> ${OUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E echo "Info: ${EXTRA_INFO}" >> ${OUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E echo "=================================================" >> ${OUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E echo "" >> ${OUT_FILE}

            VERBATIM
    )

endfunction()

function(add_symbol_sizes_postbuild TARGET_NAME)

    set(OUT_FILE "${ANALYSIS_DIR}/symbol_sizes.txt")

    fw_write_header(
            ${OUT_FILE}
            "SYMBOL SIZE REPORT"
            ${TARGET_NAME}
            "Sorted by size (descending)"
    )

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD

            COMMAND ${OBJCOPY} -R .comment $<TARGET_FILE:${TARGET_NAME}> tmp.elf

            COMMAND ${NM} -S --size-sort -r tmp.elf >> ${OUT_FILE}

            COMMAND ${CMAKE_COMMAND} -E rm -f tmp.elf

            COMMENT "Generating symbol size report"
            VERBATIM
    )

endfunction()

function(add_symbol_addresses_postbuild TARGET_NAME)

    set(OUT_FILE "${ANALYSIS_DIR}/symbol_addresses.txt")

    fw_write_header(
            ${OUT_FILE}
            "SYMBOL ADDRESS REPORT"
            ${TARGET_NAME}
            "Sorted by memory address"
    )

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD

            COMMAND ${OBJCOPY} -R .comment $<TARGET_FILE:${TARGET_NAME}> tmp.elf

            COMMAND ${NM} -S -n tmp.elf >> ${OUT_FILE}

            COMMAND ${CMAKE_COMMAND} -E rm -f tmp.elf

            COMMENT "Generating symbol address report"
            VERBATIM
    )

endfunction()

# Target: Show .text section information
function(add_text_info_target TARGET_NAME)
    if(WIN32)
        set(GREP_CMD findstr)
    else()
        set(GREP_CMD grep)
    endif()

    add_custom_target(text_info
        COMMAND ${CMAKE_COMMAND} -E echo ".text Section"
        COMMAND arm-none-eabi-objdump -t $<TARGET_FILE:${TARGET_NAME}> | ${GREP_CMD} ".text"
        DEPENDS ${TARGET_NAME}
        COMMENT "Analyzing .text section"
        VERBATIM
    )
endfunction()

# Flash Operations

# Target: Erase flash memory
function(add_erase_flash_target)
    add_custom_target(erase_flash
        COMMAND ${CMAKE_COMMAND} -E echo "Erasing flash memory..."
        COMMAND openocd
            -f custom_stm32h743xih6.cfg
            -c "reset_config none; init; reset run; halt; stm32h7x mass_erase 0; shutdown"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Erasing flash memory of STM32H743"
        VERBATIM
    )
endfunction()

# Target: Flash firmware to MCU
function(add_flash_target TARGET_NAME)
    add_custom_target(flash
        COMMAND ${CMAKE_COMMAND} -E echo "Flashing firmware..."
        COMMAND openocd
            -f custom_stm32h743xih6.cfg
            -c "program $<TARGET_FILE:${TARGET_NAME}> verify reset exit"
        DEPENDS ${TARGET_NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Flashing firmware to STM32H743"
        VERBATIM
    )
endfunction()
