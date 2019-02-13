add_sources(PLF_00_SRCS
${CMAKE_CURRENT_SOURCE_DIR}/iic/iic_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/adc/adc_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/pwm/pwm_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/timer/timer_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/uart/uart_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/dmac/dmac_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/qspi/qspi_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/hwecc/hwecc_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/rtc/rtc_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/cache/cache_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/spi/spi_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/gpio/gpio_integration.c
${CMAKE_CURRENT_SOURCE_DIR}/charger/charger.c
)

set(INC
${CMAKE_CURRENT_SOURCE_DIR}/iic
${CMAKE_CURRENT_SOURCE_DIR}/adc
${CMAKE_CURRENT_SOURCE_DIR}/pwm
${CMAKE_CURRENT_SOURCE_DIR}/timer
${CMAKE_CURRENT_SOURCE_DIR}/uart
${CMAKE_CURRENT_SOURCE_DIR}/dmac
${CMAKE_CURRENT_SOURCE_DIR}/qspi
${CMAKE_CURRENT_SOURCE_DIR}/hwecc
${CMAKE_CURRENT_SOURCE_DIR}/rtc
${CMAKE_CURRENT_SOURCE_DIR}/cache
${CMAKE_CURRENT_SOURCE_DIR}/spi
${CMAKE_CURRENT_SOURCE_DIR}/gpio
${CMAKE_CURRENT_SOURCE_DIR}/charger
)

target_include_directories(inc_00
INTERFACE ${INC}
)