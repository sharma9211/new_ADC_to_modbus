#ifndef STM32H5XX_HAL_CONF_H
#define STM32H5XX_HAL_CONF_H

#include <stdint.h>
#include <stdbool.h>

#include "stm32h5xx_hal_dma.h"
#include "stm32h5xx_hal_dma_ex.h"

/* ########################## Module Selection ############################## */
#define HAL_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED

// #define HAL_MODULE_ENABLED
// #define HAL_ADC_MODULE_ENABLED
// #define HAL_DMA_MODULE_ENABLED
// #define HAL_GPIO_MODULE_ENABLED
// #define HAL_PWR_MODULE_ENABLED
// #define HAL_RCC_MODULE_ENABLED
// #define HAL_UART_MODULE_ENABLED
// #define HAL_SPI_MODULE_ENABLED   // <-- CRITICAL



/* RTC/TIM modules disabled unless you really want RTC tick source */

/* ########################## Oscillator Values ############################# */
#define HSE_VALUE              ((uint32_t)8000000U)    /*!< External oscillator freq */
#define HSI_VALUE              ((uint32_t)64000000U)   /*!< Internal oscillator freq */
#define CSI_VALUE              ((uint32_t)4000000U)    /*!< CSI oscillator freq */
#define LSI_VALUE              ((uint32_t)32000U)      /*!< LSI oscillator freq */
#define LSE_VALUE              ((uint32_t)32768U)      /*!< LSE oscillator freq */
#define HSI48_VALUE            ((uint32_t)48000000U)   /*!< 48 MHz internal oscillator */
#define EXTERNAL_CLOCK_VALUE   ((uint32_t)12288000U)   /*!< External audio clock */

/* ########################## Timeout Values ################################ */
#define HSE_STARTUP_TIMEOUT    ((uint32_t)100U)
#define LSE_STARTUP_TIMEOUT    ((uint32_t)5000U)

/* ########################## System Configuration ########################## */
#define TICK_INT_PRIORITY      ((uint32_t)0U)   /*!< SysTick interrupt priority */

/* ########################## Assert Selection ############################## */
#define USE_FULL_ASSERT 1

/* ########################## Include Modules ############################### */
#include "stm32h5xx_hal_rcc.h"
#include "stm32h5xx_hal_gpio.h"
#include "stm32h5xx_hal_adc.h"
#include "stm32h5xx_hal_uart.h"
#include "stm32h5xx_hal_dma.h"
#include "stm32h5xx_hal_flash.h"
#include "stm32h5xx_hal_cortex.h"
#include "stm32h5xx_hal_pwr.h"
#include "stm32h5xx_hal_pwr_ex.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif

#endif /* STM32H5XX_HAL_CONF_H */
