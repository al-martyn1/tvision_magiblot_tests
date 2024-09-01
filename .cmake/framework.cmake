include_guard(GLOBAL)


if(NOT LIB_ROOT)
    message(FATAL_ERROR "LIB_ROOT is not set")
endif()


if(NOT STM32) # Win32
    set(FRAMEWORK_PALTFORM_SRC_ROOT "${LIB_ROOT}/Framework/Hardware/Win32")
    set(FRAMEWORK_PALTFORM_SOURCES "" )
    add_compile_definitions("TARGET_WIN32")
else()
    if(NOT MCU_FAMILY)
        message(FATAL_ERROR "MCU_FAMILY is not set")
    elseif(${MCU_FAMILY} STREQUAL "F1")
        set(FRAMEWORK_PALTFORM_SRC_ROOT "${LIB_ROOT}/Framework/Hardware/STM32F1xx")
        set(FRAMEWORK_PALTFORM_SOURCES
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/system_stm32f10x.c"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/dma.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/gpio.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/i2c.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/irq.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/new.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/safe_malloc.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/timer.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/uart.cpp"
           )
        set(FRAMEWORK_FREERTOS_ROOT "${LIB_ROOT}/FreeRTOS")
        #add_compile_definitions("FREERTOS_CORTEX_M3")
        add_compile_definitions("TARGET_STM32")
    elseif(${MCU_FAMILY} STREQUAL "F4")
        set(FRAMEWORK_PALTFORM_SRC_ROOT "${LIB_ROOT}/Framework/Hardware/STM32F4xx")
        set(FRAMEWORK_PALTFORM_SOURCES
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/system_stm32f4xx.c"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/asserts.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/can.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/dma.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/dma2d.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/eth_mac.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/exti.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/gpio.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/hwoccupator.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/i2c.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/irq.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/ltdc.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/new.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/rtc.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/safe_malloc.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/sdram.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/timer.cpp"
            "${FRAMEWORK_PALTFORM_SRC_ROOT}/uart.cpp"
           )
        set(FRAMEWORK_FREERTOS_ROOT "${LIB_ROOT}/FreeRTOS")
        #add_compile_definitions("FREERTOS_CORTEX_M4")
        add_compile_definitions("TARGET_STM32")
    else()
        message(FATAL_ERROR "Unknown MCU_FAMILY: ${MCU_FAMILY}")
    endif()
endif()

# include_directories("${FRAMEWORK_PALTFORM_SRC_ROOT}")

set(FRAMEWORK_ITSELF_INCLUDE_DIRECTORIES
   "${FRAMEWORK_PALTFORM_SRC_ROOT}"
   "${LIB_ROOT}/Utils"
   "${LIB_ROOT}/Framework"
   "${LIB_ROOT}/Framework/Common"
   )

set(FREERTOS_SOURCES
  "${LIB_ROOT}/FreeRTOS/croutine.c"
  "${LIB_ROOT}/FreeRTOS/event_groups.c"
  "${LIB_ROOT}/FreeRTOS/list.c"
  "${LIB_ROOT}/FreeRTOS/port.c"
  "${LIB_ROOT}/FreeRTOS/queue.c"
  "${LIB_ROOT}/FreeRTOS/safe_heap.c"
  "${LIB_ROOT}/FreeRTOS/tasks.c"
  "${LIB_ROOT}/FreeRTOS/timers.c"
)


if(FRAMEWORK_FREERTOS_ROOT)
    set(FRAMEWORK_INCLUDE_DIRECTORIES ${FRAMEWORK_ITSELF_INCLUDE_DIRECTORIES} ${FRAMEWORK_FREERTOS_ROOT})
else()
    set(FRAMEWORK_INCLUDE_DIRECTORIES ${FRAMEWORK_ITSELF_INCLUDE_DIRECTORIES}) # No FreeRTOS under Win32
endif()

if(STM32)
    set(FRAMEWORK_SOURCES 
       ${FREERTOS_SOURCES}
       ${FRAMEWORK_PALTFORM_SOURCES}
       "${LIB_ROOT}/Framework/Common/conversion.cpp"
       )
else()
    set(FRAMEWORK_SOURCES 
       ${FRAMEWORK_PALTFORM_SOURCES}
       "${LIB_ROOT}/Framework/Common/conversion.cpp"
       )
endif()

