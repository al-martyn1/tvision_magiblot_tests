include_guard(GLOBAL)

include(${CMAKE_CURRENT_LIST_DIR}/cmakepp.cmake)


# Полезные ссылки

# CMake
# https://cmake.org/cmake/help/latest/command/include_guard.html
# https://cmake.org/cmake/help/latest/command/cmake_path.html
# https://cmake.org/cmake/help/latest/variable/ENV.html
# https://cmake.org/cmake/help/latest/command/add_custom_command.html

# Примеры и хелперы для bare metal
# https://github.com/ObKo/stm32-cmake
# https://github.com/andreas-hofmann/stm32-cmake-template/blob/master/CMakeLists.txt
# https://github.com/jobroe/cmake-arm-embedded/blob/master/toolchain-arm-none-eabi.cmake

# Если будем сами писать тулчейн-файл, пригодятся следующие команды
#set(CMAKE_SYSTEM_NAME Generic)
#set(CMAKE_SYSTEM_PROCESSOR arm)
#set(CMAKE_CROSSCOMPILING 1)
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

if(NOT PRJ_ROOT)
    set(PRJ_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
endif()

if(PRJ_ROOT)
    if(NOT LIB_ROOT)
        set(LIB_ROOT "${PRJ_ROOT}/_libs")
    endif()
    if(NOT SRC_ROOT)
        set(SRC_ROOT "${PRJ_ROOT}/src")
    endif()
endif()


# Генерируем файл compile_commands.json
#add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()


# stm32-cmake почему-то не устанавливает компилятор линкером, в результате используется LD и лезут странные ошибки
#set(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_CXX}) # не нужно
set(CMAKE_LINKER ${CMAKE_CXX})


# Не нужно, stm32-cmake само их устанавливает, как приведено ниже
#set(CROSS_TARGET_OBJCOPY ${CROSS_TARGET_TRIPLET}objcopy)
#set(CROSS_TARGET_GDB ${CROSS_TARGET_TRIPLET}gdb)

# Вот так задаётся OBJCOPY/OBJDUMP/SIZE stm32-cmake'ом
#find_program(CMAKE_OBJCOPY NAMES ${STM32_TARGET_TRIPLET}-objcopy HINTS ${TOOLCHAIN_BIN_PATH})
#find_program(CMAKE_OBJDUMP NAMES ${STM32_TARGET_TRIPLET}-objdump HINTS ${TOOLCHAIN_BIN_PATH})
#find_program(CMAKE_SIZE NAMES ${STM32_TARGET_TRIPLET}-size HINTS ${TOOLCHAIN_BIN_PATH})


if (STM32_TARGET_TRIPLET)
    set(STM32 1)
else()
    set(STM32 0)
endif()


if(STM32)
    message(STATUS "STM32 defined and not false")
    set(TARGET_STM32 1)
    # Глобально задаём для всех целей
    add_compile_definitions("TARGET_STM32")
else()
    message(NOTICE "STM32 NOT defined or false, using WIN32 target")
    set(TARGET_WIN32 1)
    # Глобально задаём для всех целей
    add_compile_definitions("TARGET_WIN32" "_WIN32" "WIN32")

    set(CMAKE_EXECUTABLE_SUFFIX_C   .exe)
    set(CMAKE_EXECUTABLE_SUFFIX_CXX .exe)
    set(CMAKE_EXECUTABLE_SUFFIX_ASM .exe)
endif()


if(STM32)

    if(NOT MCU)
        message(FATAL_ERROR "MCU is not set")
    endif()

    string_starts_with(${MCU} "STM32")
    ans(IS_MCU_STM32)

    if(IS_MCU_STM32)

        #set(MCU_FAMILY string_slice(${MCU} 5 7))
        # Так нихрена не работает, 
        # MCU_FAMILY устанавливается в
        # "string_slice" "(" "STM32F103C8" "5" "7" ")"
        # надо как ниже, через ans

        string_slice(${MCU} 5 7)
        ans(MCU_FAMILY)

        # message("MCU (${MCU}) starts with STM32")
        # set(MCU_FAMILY )
        message("MCU_FAMILY: ${MCU_FAMILY}")

        
        if(${MCU_FAMILY} STREQUAL "F0")
        elseif(${MCU_FAMILY} STREQUAL "F1")
            set(MCU_HAL_DRIVER_DIR_NAME "STM32F1xx_HAL_Driver")
            set(MCU_SPL_DRIVER_DIR_NAME "STM32F1xx_StdPeriph_Driver")
        elseif(${MCU_FAMILY} STREQUAL "F2")
        elseif(${MCU_FAMILY} STREQUAL "F3")
        elseif(${MCU_FAMILY} STREQUAL "F4")
            set(MCU_HAL_DRIVER_DIR_NAME "STM32F4xx_HAL_Driver")
            set(MCU_SPL_DRIVER_DIR_NAME "STM32F4xx_StdPeriph_Driver")
        else()
            message(FATAL_ERROR "Unknown MCU_FAMILY: ${MCU_FAMILY}")
        endif()

    else()
        message("MCU (${MCU}) not starts with STM32")
    endif()

endif()


# HAL_DRIVER_ROOT points to
# Drivers
#     STM32F1xx_HAL_Driver
#     STM32F4xx_HAL_Driver
#     ...
# Sets MCU_HAL_SRC_ROOT, MCU_HAL_INCLUDE and MCU_HAL_SOURCES vars in parent scope
#      
function(fw_stm32_init_mcu_hal_sources HAL_DRIVER_ROOT)

    if(NOT MCU_FAMILY)
        message(FATAL_ERROR "MCU_FAMILY not defined")
    endif()

    if(NOT MCU_HAL_DRIVER_DIR_NAME)
        message(FATAL_ERROR "MCU_HAL_DRIVER_DIR_NAME not defined")
    endif()

    #message(NOTICE "HAL_DRIVER_ROOT: ${HAL_DRIVER_ROOT}")

    set(MCU_HAL_SRC_ROOT "${HAL_DRIVER_ROOT}/${MCU_HAL_DRIVER_DIR_NAME}/Src")
    set(MCU_HAL_SRC_ROOT "${HAL_DRIVER_ROOT}/${MCU_HAL_DRIVER_DIR_NAME}/Src" PARENT_SCOPE)
    set(MCU_HAL_INCLUDE  "${HAL_DRIVER_ROOT}/${MCU_HAL_DRIVER_DIR_NAME}/Inc" PARENT_SCOPE)

    if(${MCU_FAMILY} STREQUAL "F0")
    elseif(${MCU_FAMILY} STREQUAL "F1")
        set(MCU_HAL_SOURCES
           )
    elseif(${MCU_FAMILY} STREQUAL "F2")
    elseif(${MCU_FAMILY} STREQUAL "F3")
    elseif(${MCU_FAMILY} STREQUAL "F4")
        set(MCU_HAL_SOURCES
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_adc.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_adc_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_can.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_cec.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_cortex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_crc.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_cryp.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_cryp_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_dac.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_dac_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_dcmi.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_dcmi_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_dfsdm.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_dma.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_dma_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_dma2d.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_dsi.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_eth.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_exti.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_flash.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_flash_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_flash_ramfunc.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_fmpi2c.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_fmpi2c_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_fmpsmbus.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_fmpsmbus_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_gpio.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_hash.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_hash_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_hcd.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_i2c.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_i2c_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_i2s.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_i2s_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_irda.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_iwdg.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_lptim.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_ltdc.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_ltdc_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_mmc.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_msp_template.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_nand.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_nor.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_pccard.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_pcd.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_pcd_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_pwr.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_pwr_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_qspi.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_rcc.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_rcc_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_rng.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_rtc.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_rtc_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_sai.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_sai_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_sd.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_sdram.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_smartcard.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_smbus.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_spdifrx.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_spi.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_sram.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_tim.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_tim_ex.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_timebase_rtc_alarm_template.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_timebase_rtc_wakeup_template.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_timebase_tim_template.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_uart.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_usart.c
            ${MCU_HAL_SRC_ROOT}/stm32f4xx_hal_wwdg.c
            PARENT_SCOPE
           )
    else()
        message(FATAL_ERROR "Unknown MCU_FAMILY: ${MCU_FAMILY}")
    endif()

endfunction()


# LL_DRIVER_ROOT points to
# Drivers
#     STM32F1xx_HAL_Driver
#     STM32F4xx_HAL_Driver
#     ...
# Sets MCU_LL_SRC_ROOT, MCU_LL_INCLUDE and MCU_LL_SOURCES vars in parent scope
#      
function(fw_stm32_init_mcu_ll_sources LL_DRIVER_ROOT)

    if(NOT MCU_FAMILY)
        message(FATAL_ERROR "MCU_FAMILY not defined")
    endif()

    if(NOT MCU_HAL_DRIVER_DIR_NAME)
        message(FATAL_ERROR "MCU_HAL_DRIVER_DIR_NAME not defined")
    endif()

    set(MCU_LL_SRC_ROOT "${LL_DRIVER_ROOT}/${MCU_HAL_DRIVER_DIR_NAME}/Src")
    set(MCU_LL_SRC_ROOT "${LL_DRIVER_ROOT}/${MCU_HAL_DRIVER_DIR_NAME}/Src" PARENT_SCOPE)
    set(MCU_LL_INCLUDE  "${LL_DRIVER_ROOT}/${MCU_HAL_DRIVER_DIR_NAME}/Inc" PARENT_SCOPE)


    if(${MCU_FAMILY} STREQUAL "F0")
    elseif(${MCU_FAMILY} STREQUAL "F1")
        set(MCU_LL_SOURCES
            ""
            PARENT_SCOPE
           )
    elseif(${MCU_FAMILY} STREQUAL "F2")
    elseif(${MCU_FAMILY} STREQUAL "F3")
    elseif(${MCU_FAMILY} STREQUAL "F4")
        set(MCU_LL_SOURCES
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_adc.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_crc.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_dac.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_dma.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_dma2d.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_exti.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_fmc.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_fmpi2c.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_fsmc.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_gpio.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_i2c.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_lptim.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_pwr.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_rcc.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_rng.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_rtc.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_sdmmc.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_spi.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_tim.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_usart.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_usb.c
            ${MCU_LL_SRC_ROOT}/stm32f4xx_ll_utils.c
            PARENT_SCOPE
           )
    else()
        message(FATAL_ERROR "Unknown MCU_FAMILY: ${MCU_FAMILY}")
    endif()

endfunction()




# SPL_DRIVER_ROOT points to
# Drivers
#     STM32F4xx_StdPeriph_Driver
#     STM32F1xx_StdPeriph_Driver
#     ...
# Sets MCU_SPL_SRC_ROOT, MCU_SPL_INCLUDE and MCU_SPL_SOURCES vars in parent scope
#      
function(fw_stm32_init_mcu_spl_sources SPL_DRIVER_ROOT)

    if(NOT MCU_FAMILY)
        message(FATAL_ERROR "MCU_FAMILY not defined")
    endif()

    if(NOT MCU_SPL_DRIVER_DIR_NAME)
        message(FATAL_ERROR "MCU_SPL_DRIVER_DIR_NAME not defined")
    endif()

    set(MCU_SPL_SRC_ROOT "${SPL_DRIVER_ROOT}/${MCU_SPL_DRIVER_DIR_NAME}/src")
    set(MCU_SPL_SRC_ROOT "${SPL_DRIVER_ROOT}/${MCU_SPL_DRIVER_DIR_NAME}/src" PARENT_SCOPE)
    set(MCU_SPL_INCLUDE  "${SPL_DRIVER_ROOT}/${MCU_SPL_DRIVER_DIR_NAME}/inc" PARENT_SCOPE)


    if(${MCU_FAMILY} STREQUAL "F0")
    elseif(${MCU_FAMILY} STREQUAL "F1")
        set(MCU_SPL_SOURCES
            ""
            PARENT_SCOPE
           )
    elseif(${MCU_FAMILY} STREQUAL "F2")
    elseif(${MCU_FAMILY} STREQUAL "F3")
    elseif(${MCU_FAMILY} STREQUAL "F4")
        set(MCU_SPL_SOURCES
            ${MCU_SPL_SRC_ROOT}/misc.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_adc.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_can.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_cec.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_crc.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_cryp.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_cryp_aes.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_cryp_des.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_cryp_tdes.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_dac.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_dbgmcu.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_dcmi.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_dfsdm.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_dma.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_dma2d.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_dsi.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_exti.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_flash.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_flash_ramfunc.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_fmc.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_fmpi2c.c
            # ${MCU_SPL_SRC_ROOT}/stm32f4xx_fsmc.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_gpio.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_hash.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_hash_md5.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_hash_sha1.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_i2c.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_iwdg.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_lptim.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_ltdc.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_pwr.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_qspi.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_rcc.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_rng.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_rtc.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_sai.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_sdio.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_spdifrx.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_spi.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_syscfg.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_tim.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_usart.c
            ${MCU_SPL_SRC_ROOT}/stm32f4xx_wwdg.c
            PARENT_SCOPE
           )
    else()
        message(FATAL_ERROR "Unknown MCU_FAMILY: ${MCU_FAMILY}")
    endif()

endfunction()






# Извлекаем переменную OPENOCD_ROOT
if(STM32)
    # cmake_path довольно новая команда, в 3.17 её ещё нет
    #cmake_path(CONVERT ${ENV_OPENOCD_ROOT} TO_CMAKE_PATH_LIST OPENOCD_ROOT NORMALIZE)

    if(NOT DEFINED OPENOCD_ROOT)
        message(STATUS "OPENOCD_ROOT not defined, trying to use environment variable OPENOCD_ROOT")
        if(DEFINED ENV{OPENOCD_ROOT})
            #set(OPENOCD_ROOT $ENV{OPENOCD_ROOT})
        endif()
    endif()

    if(NOT DEFINED OPENOCD_ROOT)

        #message(FATAL_ERROR "OPENOCD_ROOT not defined, and env var OPENOCD_ROOT is also undefined")
        message(NOTICE "OPENOCD_ROOT not defined, and env var OPENOCD_ROOT is also undefined")

        if(WIN32 OR CYGWIN)
          set(OPENOCD openocd.exe)
        else()
          set(OPENOCD openocd)
        endif()

    else()

        if(DEFINED ENV{OPENOCD_SHARE})
            set(OPENOCD_SHARE $ENV{OPENOCD_SHARE})
        else()
            set(OPENOCD_SHARE "${OPENOCD_ROOT}/share")
            message(STATUS "No OPENOCD_SHARE env var specified, using default OPENOCD_ROOT/share: " ${OPENOCD_SHARE})
        endif()
    
        if(DEFINED ENV{OPENOCD_SCRIPTS})
            set(OPENOCD_SCRIPTS $ENV{OPENOCD_SCRIPTS})
        else()
            set(OPENOCD_SCRIPTS "${OPENOCD_SHARE}/openocd/scripts")
            message(STATUS "No OPENOCD_SCRIPTS env var specified, using default OPENOCD_SHARE/openocd/scripts: " ${OPENOCD_SCRIPTS})
        endif()
    
        if(WIN32 OR CYGWIN)
          set(OPENOCD ${OPENOCD_ROOT}/bin/openocd.exe)
        else()
          set(OPENOCD ${OPENOCD_ROOT}/bin/openocd)
        endif()

    endif()

endif()



# stm32-cmake функции для генерации HEX/BIN файлов из эльфа
#stm32_generate_binary_file(TARGET)
#stm32_generate_hex_file(TARGET)


function(fw_stm32_setup_mcu_options)

    if(NOT MCU)
        message(FATAL_ERROR "MCU is not set")
    endif()

    set(IS_MCU_STM32 string_starts_with(${MCU} "STM32"))

    if(IS_MCU_STM32)

        #set(MCU_FAMILY string_slice(${MCU} 5 7))
        # Так нихрена не работает, 
        # MCU_FAMILY устанавливается в
        # "string_slice" "(" "STM32F103C8" "5" "7" ")"
        # надо как ниже, через ans
        string_slice(${MCU} 5 7)
        ans(MCU_FAMILY)

        # message("MCU (${MCU}) starts with STM32")
        # set(MCU_FAMILY )
        message("MCU_FAMILY: ${MCU_FAMILY}")

    else()
        message("MCU (${MCU}) not starts with STM32")
    endif()


    if(NOT MCU_FAMILY)
        message(FATAL_ERROR "MCU_FAMILY is not set")
    endif()

    set(MCU_COMPILE_DEFINITIONS ${MCU})


    if(${MCU_FAMILY} STREQUAL "F0")
        set(MCU_CORE "cortex-m0")
        list(APPEND MCU_COMPILE_DEFINITIONS "MCU_FAMILY_STM32F0")
        # target_compile_definitions(${TARGET} PRIVATE "MCU_FAMILY_STM32F0")
    elseif(${MCU_FAMILY} STREQUAL "F1")
        set(MCU_CORE "cortex-m3")
        # target_compile_definitions(${TARGET} PRIVATE "MCU_FAMILY_STM32F1")
        list(APPEND MCU_COMPILE_DEFINITIONS "MCU_FAMILY_STM32F1")
    elseif(${MCU_FAMILY} STREQUAL "F2")
        set(MCU_CORE "cortex-m3")
        # target_compile_definitions(${TARGET} PRIVATE "MCU_FAMILY_STM32F2")
        list(APPEND MCU_COMPILE_DEFINITIONS "MCU_FAMILY_STM32F2")
    elseif(${MCU_FAMILY} STREQUAL "F3")
        set(MCU_CORE "cortex-m4")
        set(MCU_FPU  "fpv4-sp-d16")
        set(MCU_FLOAT_ABI "hard")
        # target_compile_definitions(${TARGET} PRIVATE "MCU_FAMILY_STM32F3")
        list(APPEND MCU_COMPILE_DEFINITIONS "MCU_FAMILY_STM32F3")
    elseif(${MCU_FAMILY} STREQUAL "F4")
        set(MCU_CORE "cortex-m4")
        set(MCU_FPU  "fpv4-sp-d16")
        set(MCU_FLOAT_ABI "hard")
        # target_compile_definitions(${TARGET} PRIVATE "MCU_FAMILY_STM32F4")
        list(APPEND MCU_COMPILE_DEFINITIONS "MCU_FAMILY_STM32F4")
    else()
        message(FATAL_ERROR "Unknown MCU_FAMILY: ${MCU_FAMILY}")
    endif()

    set(MCU_COMPILE_OPTIONS "--specs=nosys.specs" "-mcpu=${MCU_CORE}" "-mthumb")
    set(MCU_LINK_OPTIONS    "-Wl,--no-warn-rwx-segments" "--specs=nosys.specs" "-mcpu=${MCU_CORE}" "-mthumb")

    #target_link_options(${TARGET} PRIVATE "-Wl,--no-warn-rwx-segments")
    #target_compile_options(${TARGET} PRIVATE "--specs=nosys.specs")
    #target_link_options(${TARGET} PRIVATE "--specs=nosys.specs")

    # target_compile_options(${TARGET} PRIVATE "-mcpu=${MCU_CORE}" "-mthumb")
    # target_link_options(${TARGET} PRIVATE "-mcpu=${MCU_CORE}" "-mthumb")

    if(MCU_FPU)
        #target_compile_options(${TARGET} PRIVATE "-mfpu=${MCU_FPU}")
        #target_link_options(${TARGET} PRIVATE "-mfpu=${MCU_FPU}")
        list(APPEND MCU_COMPILE_OPTIONS "-mfpu=${MCU_FPU}")
        list(APPEND MCU_LINK_OPTIONS    "-mfpu=${MCU_FPU}")
    endif()

    if(MCU_FLOAT_ABI)
        #target_compile_options(${TARGET} PRIVATE "-mfloat-abi=${MCU_FLOAT_ABI}")
        #target_link_options(${TARGET} PRIVATE "-mfloat-abi=${MCU_FLOAT_ABI}")
        list(APPEND MCU_COMPILE_OPTIONS "-mfloat-abi=${MCU_FLOAT_ABI}")
        list(APPEND MCU_LINK_OPTIONS    "-mfloat-abi=${MCU_FLOAT_ABI}")
    endif()

    set(MCU_COMPILE_DEFINITIONS ${MCU_COMPILE_DEFINITIONS} PARENT_SCOPE)
    set(MCU_COMPILE_OPTIONS     ${MCU_COMPILE_OPTIONS} PARENT_SCOPE)
    set(MCU_LINK_OPTIONS        ${MCU_LINK_OPTIONS} PARENT_SCOPE)

endfunction()


function(fw_stm32_setup_target_options TARGET)

    fw_stm32_setup_mcu_options()

    #[[
    if(${MCU_FAMILY} STREQUAL "F0")
        set(MCU_CORE "cortex-m0")
        target_compile_definitions(${TARGET} PRIVATE "MCU_FAMILY_STM32F0")
    elseif(${MCU_FAMILY} STREQUAL "F1")
        set(MCU_CORE "cortex-m3")
        target_compile_definitions(${TARGET} PRIVATE "MCU_FAMILY_STM32F1")
    elseif(${MCU_FAMILY} STREQUAL "F2")
        set(MCU_CORE "cortex-m3")
        target_compile_definitions(${TARGET} PRIVATE "MCU_FAMILY_STM32F2")
    elseif(${MCU_FAMILY} STREQUAL "F3")
        set(MCU_CORE "cortex-m4")
        set(MCU_FPU  "fpv4-sp-d16")
        set(MCU_FLOAT_ABI "hard")
        target_compile_definitions(${TARGET} PRIVATE "MCU_FAMILY_STM32F3")
    elseif(${MCU_FAMILY} STREQUAL "F4")
        set(MCU_CORE "cortex-m4")
        set(MCU_FPU  "fpv4-sp-d16")
        set(MCU_FLOAT_ABI "hard")
        target_compile_definitions(${TARGET} PRIVATE "MCU_FAMILY_STM32F4")
    else()
        message(FATAL_ERROR "Unknown MCU_FAMILY: ${MCU_FAMILY}")
    endif()
    ]]


    #message(NOTICE "TARGET: ${TARGET}")
    # target_compile_definitions(${TARGET} PRIVATE "${MCU}")

    #[[
    target_link_options(${TARGET} PRIVATE "-Wl,--no-warn-rwx-segments")
    target_compile_options(${TARGET} PRIVATE "--specs=nosys.specs")
    target_link_options(${TARGET} PRIVATE "--specs=nosys.specs")

    target_compile_options(${TARGET} PRIVATE "-mcpu=${MCU_CORE}" "-mthumb")
    target_link_options(${TARGET} PRIVATE "-mcpu=${MCU_CORE}" "-mthumb")

    if(MCU_FPU)
        target_compile_options(${TARGET} PRIVATE "-mfpu=${MCU_FPU}")
        target_link_options(${TARGET} PRIVATE "-mfpu=${MCU_FPU}")
    endif()

    if(MCU_FLOAT_ABI)
        target_compile_options(${TARGET} PRIVATE "-mfloat-abi=${MCU_FLOAT_ABI}")
        target_link_options(${TARGET} PRIVATE "-mfloat-abi=${MCU_FLOAT_ABI}")
    endif()
    ]]

    target_compile_definitions(${TARGET} PRIVATE ${MCU_COMPILE_DEFINITIONS})
    target_compile_options(${TARGET} PRIVATE ${MCU_COMPILE_OPTIONS})
    target_link_options(${TARGET} PRIVATE ${MCU_LINK_OPTIONS})


    # Про линкер скрипты - https://www.stf12.org/developers/freerots_ec-linker_script.html
    target_link_options(${TARGET} PRIVATE "-Wl,-T${LINKER_SCRIPT}")

    # Про HEX - https://en.wikipedia.org/wiki/Intel_HEX

endfunction()


function(fw_win32_setup_target_console TARGET)
    target_compile_definitions(${TARGET} PRIVATE "CONSOLE" "_CONSOLE")
endfunction()

function(fw_win32_setup_target_unicode TARGET)
    target_compile_definitions(${TARGET} PRIVATE "UNICODE" "_UNICODE")
endfunction()


#TODO: !!! Сделать функцию, которая настраивает OPEN_OCD и делает вирт таргеты или как-то так 
# для заливки прошивки и старта отладки

# C:\OpenOCD\bin\openocd -f C:\OpenOCD\share\openocd\scripts\interface\stlink-v2.cfg -f C:\OpenOCD\share\openocd\scripts\target\stm32f1x.cfg 

#https://github.com/andreas-hofmann/stm32-cmake-template/blob/master/CMakeLists.txt


# Про openocd - https://crtv-club.readthedocs.io/ru/latest/nrf52_stlink_flashing.html
# https://habr.com/ru/articles/439148/
# Commands - https://openocd.org/doc/html/General-Commands.html

# https://electronix.ru/forum/topic/171010-proshit-gd32f407-cherez-st-link-pri-pomoschi-openocd/
function(fw_stm32_add_flash_target TARGET INTERFACE TARGET_CONFIG)

    if(NOT OPENOCD)
        message(FATAL_ERROR "OPENOCD is not set, please set OPENOCD_ROOT var or OPENOCD_ROOT env var")
    endif()

    #NOT INTERFACE OR
    #if(${INTERFACE} STREQUAL "")
    if(INTERFACE STREQUAL "")
        set(INTERFACE "stlink")
    endif()

    #${OPENOCD_SCRIPTS}/ - не нужно, interface и target  нормально находит и так
    add_custom_command(TARGET ${TARGET} POST_BUILD
         COMMAND ${OPENOCD} -f interface/${INTERFACE}.cfg -f ${TARGET_CONFIG} -c "program $<TARGET_FILE_BASE_NAME:${TARGET}>.hex verify reset exit"
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
         DEPENDS $<TARGET_FILE_BASE_NAME:${TARGET}>.elf
         )
endfunction()


# "UNICODE" "CONSOLE" "BIN" "HEX" "FLASH"
function(fw_add_target_options TARGET)

    if(STM32)
        message(NOTICE "Configuring executable for STM32")
    else() # Win32
        message(NOTICE "Configuring executable for WIN32")
    endif()

    if(STM32)
        fw_stm32_setup_target_options(${TARGET})
    endif()


    # https://jeremimucha.com/2021/02/cmake-functions-and-macros/

    #math(EXPR indices "${ARGC} - 1")
    #foreach(index RANGE ${indices})

    math(EXPR maxArgN "${ARGC} - 1")
    foreach(index RANGE 1 ${maxArgN} 1)

        #message("  ARGV${index}: ${ARGV${index}}")

        if(${ARGV${index}} STREQUAL "UNICODE")
            if(STM32)
            else()
                message(NOTICE "Application ${TARGET} is Unicode")
                fw_win32_setup_target_unicode(${TARGET})
            endif()

        elseif(${ARGV${index}} STREQUAL "CONSOLE")
            if(STM32)
            else()
                message(NOTICE "Application ${TARGET} is Console")
                fw_win32_setup_target_console(${TARGET})
            endif()

        elseif(${ARGV${index}} STREQUAL "BIN")
            if(STM32)
                message(NOTICE "Generate BIN file ${TARGET}.bin")
                stm32_generate_binary_file(${TARGET})
            else()
            endif()

        elseif(${ARGV${index}} STREQUAL "BIGOBJ")
            if(STM32)
                # Not supported for ARM target
            else()
                message(NOTICE "Setting BIGOBJ (-Wa,-mbig-obj) option for ${TARGET}")
                target_compile_options(${TARGET} PRIVATE -Wa,-mbig-obj)
            endif()

        elseif(${ARGV${index}} STREQUAL "HEX")
            if(STM32)
                if(NOT DEFINED HEX_TARGET_OPTION_ALREADY)
                    message(NOTICE "Generate HEX file ${TARGET}.hex")
                    stm32_generate_hex_file(${TARGET})
                    set(HEX_TARGET_OPTION_ALREADY 1)
                endif()
            else()
            endif()

        elseif(${ARGV${index}} STREQUAL "FLASH")
            if(STM32)
                message(NOTICE "Generate HEX file ${TARGET}.hex and flash it to device")
                if(NOT DEFINED HEX_TARGET_OPTION_ALREADY)
                    stm32_generate_hex_file(${TARGET})
                    set(HEX_TARGET_OPTION_ALREADY 1)
                endif()
                fw_stm32_add_flash_target(${TARGET} "${STM32_PROGRAMMER}" "${OPENOCD_TARGET_CONFIG}")
            else()
            endif()

        else()
            message(FATAL_ERROR "Unknown target option: ${ARGV${index}}")
        endif()

    endforeach()

endfunction()


