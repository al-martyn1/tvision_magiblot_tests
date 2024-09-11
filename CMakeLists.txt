cmake_minimum_required(VERSION 3.17)

project(tv-test C CXX ASM)


# CMake options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)


# Fill in the target MCU here
# if(STM32)
    set(MCU "STM32F429II")
# endif()  # STM32


include(.cmake/common.cmake)
# Framework нам пока не нужен
# include(.cmake/framework.cmake)

# Настройка TurboVision под терминал
if(STM32)
    set(TV_BARE_METAL ON)
    set(TV_NO_NCURSES ON)
    set(TV_BARE_METAL_DISPLAY_TERMINAL ON)
    set(TV_NO_FILESYSTEM 1)
    set(TV_NO_LOCALE_SUPPORT 1)
    fw_stm32_setup_mcu_options()
    set(TV_MCU_COMPILE_DEFINITIONS ${MCU_COMPILE_DEFINITIONS})
    set(TV_MCU_COMPILE_OPTIONS     ${MCU_COMPILE_OPTIONS})
    set(TV_MCU_LINK_OPTIONS        ${MCU_LINK_OPTIONS})
    add_compile_definitions(${MCU_COMPILE_DEFINITIONS})
    add_compile_options(${MCU_COMPILE_OPTIONS})
    add_link_options(${MCU_LINK_OPTIONS})
endif()


# set(TV_OPTIMIZE_BUILD  OFF)
set(TV_REDUCE_APP_SIZE ON)
set(TV_USE_STATIC_RTL ON)
set(TV_BUILD_USING_GPM OFF) # prevent message: (tvision) 'gpm' library requested but not found

# Disable some warnings in TurboVision
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-Wno-psabi")
endif() # GNU


if(NOT STM32)
    add_compile_definitions(USE_UMBA_LOUT_COUT)
else()
    add_compile_definitions(TV_DISABLE_TV_PLATFORM_STRINGS_STRUPR)
    add_compile_definitions(TV_DISABLE_TV_PLATFORM_STRINGS_ITOA)
endif()  # STM32


# Подсасываем библиотеки
if(STM32)
    add_subdirectory("${LIB_ROOT}/arm_none_eabi_crt_stub")
endif()
add_subdirectory("${LIB_ROOT}/tvision")
add_subdirectory("${LIB_ROOT}/sfmt")
add_subdirectory("${LIB_ROOT}/umba")


# C/C++ language settings
# https://matgomes.com/setting-cpp-standard-cmake/
set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS Off)
set(CMAKE_CXX_EXTENSIONS Off)


set(COMMON_LIBS)
list(APPEND COMMON_LIBS sfmt::sfmt umba::umba tvision::tvision)
if(STM32)
    list(APPEND COMMON_LIBS arm_none_eabi_crt_stub::arm_none_eabi_crt_stub)
endif()



include_directories(${LIB_ROOT})

# Project include paths
set(INCLUDES
)

set(EXAMPLES ${LIB_ROOT}/tvision/tvision/examples)

set(SOURCES # test.cpp
#    tvision/examples/progba/example.cpp
#    tvision/examples/progba/makerez.cpp
#    tvision/examples/progba/readrez.cpp
#    tvision/examples/progba/tprogbar.cpp
)


if(STM32)
endif()  # STM32

# https://stackoverflow.com/questions/11944060/how-to-detect-target-architecture-using-cmake
message("Target Processor: " ${CMAKE_SYSTEM_PROCESSOR})
message("Host Processor  : " ${CMAKE_HOST_SYSTEM_PROCESSOR})
 


add_compile_definitions(${TV_DEFINITIONS})


if(STM32)
    # Missing linker script cause 
    # "examples/progba/example.cpp.obj:1: ignoring invalid character `\177' in script"
    # "syntax error"
    # "ld returned 1 exit status"
    set(LINKER_SCRIPT "${SRC_ROOT}/scripts/STM32F429_no_bootloader.lds")

endif()


#-------

if(NOT STM32)

#------- ansitermseqparser_test
set(TARGET ansitermseqparser_test)
add_executable(${TARGET} ${SRC_ROOT}/${TARGET}.cpp ) # ${SRC_ROOT}/crt_stub.c
fw_add_target_options(${TARGET} "UNICODE" "CONSOLE" "BIGOBJ"
                              #"BIN" "HEX"
                              # "FLASH"
                              )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")



#------- ansitermseqtriegen
set(TARGET ansitermseqtriegen)
add_executable(${TARGET} ${SRC_ROOT}/${TARGET}.cpp ) # ${SRC_ROOT}/crt_stub.c
fw_add_target_options(${TARGET} "UNICODE" "CONSOLE" "BIGOBJ"
                              #"BIN" "HEX"
                              # "FLASH"
                              )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


#------- ansitermseqtriegraphgen
set(TARGET ansitermseqtriegraphgen)
add_executable(${TARGET} ${SRC_ROOT}/${TARGET}.cpp ) # ${SRC_ROOT}/crt_stub.c
fw_add_target_options(${TARGET} "UNICODE" "CONSOLE" "BIGOBJ"
                              #"BIN" "HEX"
                              # "FLASH"
                              )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")

#-------

endif()

#------- example
set(TARGET example)
add_executable(${TARGET} ${SRC_ROOT}/tv_test/${TARGET}.cpp ${SRC_ROOT}/tv_test/tprogbar.cpp ) # ${SRC_ROOT}/crt_stub.c
fw_add_target_options(${TARGET} "UNICODE" "CONSOLE" "BIGOBJ"
                              #"BIN" "HEX"
                              # "FLASH"
                              )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")
if(STM32)
    target_compile_definitions(${TARGET} PRIVATE "HSE_VALUE=16000000" "MCU_PLL_M=16" "MCU_PLL_Q=7" "MCU_PLL_N=360" "MCU_PLL_P=2")
endif()



# add_executable(makerez ${EXAMPLES}/progba/makerez.cpp ${EXAMPLES}/progba/tprogbar.cpp ) # ${SRC_ROOT}/crt_stub.c
# fw_add_target_options(makerez "UNICODE" "CONSOLE" "BIGOBJ"
#                               #"BIN" "HEX"
#                               # "FLASH"
#                               )
# target_link_libraries(makerez PRIVATE "${COMMON_LIBS}")
# if(STM32)
#     target_compile_definitions(example PRIVATE "HSE_VALUE=16000000" "MCU_PLL_M=16" "MCU_PLL_Q=7" "MCU_PLL_N=360" "MCU_PLL_P=2")
# endif()


# add_executable(readrez ${EXAMPLES}/progba/readrez.cpp ${EXAMPLES}/progba/tprogbar.cpp ) # ${SRC_ROOT}/crt_stub.c
# fw_add_target_options(readrez "UNICODE" "CONSOLE" "BIGOBJ"
#                               #"BIN" "HEX"
#                               # "FLASH"
#                               )
# target_link_libraries(readrez PRIVATE "${COMMON_LIBS}")
# if(STM32)
#     target_compile_definitions(example PRIVATE "HSE_VALUE=16000000" "MCU_PLL_M=16" "MCU_PLL_Q=7" "MCU_PLL_N=360" "MCU_PLL_P=2")
# endif()




# target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})

# target_compile_definitions(${PROJECT_NAME} PRIVATE "ES_4007_BMCL")
# target_compile_definitions(${PROJECT_NAME} PRIVATE "FIRMWARE_TEST_MODE")

# target_link_libraries(${PROJECT_NAME} PRIVATE "${COMMON_LIBS}")

# if(STM32)
#     # Настраиваем TheFramework
#     target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--defsym=FW_HDR_FLAGS=0x7")
#     
#     # Настраиваем раскладку флешки/срама
#     target_compile_definitions(${PROJECT_NAME} PRIVATE "flash_layout")
#     target_compile_definitions(${PROJECT_NAME} PRIVATE "sram_layout")
#     
#     # Настраиваем тактирование
#     target_compile_definitions(${PROJECT_NAME} PRIVATE "HSE_VALUE=16000000" "MCU_PLL_M=16" "MCU_PLL_Q=7" "MCU_PLL_N=360" "MCU_PLL_P=2")
# endif()










