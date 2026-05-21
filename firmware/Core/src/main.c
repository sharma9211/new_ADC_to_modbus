#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_spi.h"
#include "stm32h5xx_hal_uart.h"
#include "stm32h5xx_hal_adc.h"
#include <stdio.h>
#include "modbus.h"

SPI_HandleTypeDef hspi4;
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_adc1;

/* Function prototypes */
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_ADC1_Init(void);
void MX_USART3_UART_Init(void);
void MX_SPI4_Init(void);
uint32_t ADS127L14_ReadSample(void);
void ADS127L14_Reset(void);
void ADS127L14_Start(void);
void ADS127L14_ConfigMode(void);
void Error_Handler(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_USART3_UART_Init();
    MX_SPI4_Init();

    ADS127L14_Reset();
    ADS127L14_ConfigMode();
    ADS127L14_Start();

    Modbus_Init(&huart3);

    while (1)
    {
        uint32_t sample = ADS127L14_ReadSample();
        modbus_registers[0] = (uint16_t)(sample & 0xFFFF);
        modbus_registers[1] = (uint16_t)((sample >> 16) & 0xFFFF);

        Modbus_Poll();
        printf("ADC Sample: %lu\n", sample);
        HAL_Delay(1000);
    }
}

// /* --- Function definitions --- */
// void SystemClock_Config(void) {
//     // TODO: configure PLL, HSE, etc.
// }

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure the main external oscillator (HSE) */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 2;      // Divider
    RCC_OscInitStruct.PLL.PLLN = 125;    // Multiplier
    RCC_OscInitStruct.PLL.PLLP = 2;      // PLLP output
    RCC_OscInitStruct.PLL.PLLQ = 2;      // PLLQ output
    RCC_OscInitStruct.PLL.PLLR = 2;      // PLLR output
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    // User can add error handling code here
    // For now, just loop forever
    __disable_irq();
    while (1)
    {
        // Optionally blink an LED or send debug message
    }
}

void MX_DMA_Init(void) {
    // TODO: configure DMA
}

void MX_ADC1_Init(void) {
    // TODO: configure ADC1 (if you want STM32 internal ADC)
}

// void MX_USART3_UART_Init(void) {
//     // TODO: configure USART3 (for Modbus / printf output)
//     huart3.Instance = USART3;
//     huart3.Init.BaudRate = 9600; // Modbus RTU standard
//     huart3.Init.WordLength = UART_WORDLENGTH_8B;
//     huart3.Init.StopBits = UART_STOPBITS_1;
//     huart3.Init.Parity = UART_PARITY_NONE;
//     huart3.Init.Mode = UART_MODE_TX_RX;
//     huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//     huart3.Init.OverSampling = UART_OVERSAMPLING_16;
//     HAL_UART_Init(&huart3);
// }

void MX_USART3_UART_Init(void) {
    huart3.Instance = USART3;

    // Baud rate: choose 9600 or 115200
    huart3.Init.BaudRate = 9600;     // Safe, reliable for Modbus RTU
    // huart3.Init.BaudRate = 115200; // High-speed, for fast ADC streaming

    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart3) != HAL_OK) {
        Error_Handler();
    }
}

/* --- SPI4 init for ADS127L14 --- */
void MX_SPI4_Init(void) {
    hspi4.Instance = SPI4;
    hspi4.Init.Mode = SPI_MODE_MASTER;
    hspi4.Init.Direction = SPI_DIRECTION_2LINES;
    hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi4.Init.CLKPhase = SPI_PHASE_2EDGE; // CPHA=1
    hspi4.Init.NSS = SPI_NSS_SOFT;
    hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi4.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    HAL_SPI_Init(&hspi4);
}


/* --- ADS127L14 sample read --- */
// uint32_t ADS127L14_ReadSample(void) {
//     uint8_t rxBuf[3];
//     //HAL_SPI_Receive(&hspi4, rxBuf, 3, HAL_MAX_DELAY);
//     HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
//     HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);

//     return ((uint32_t)rxBuf[0] << 16) |
//            ((uint32_t)rxBuf[1] << 8) |
//            ((uint32_t)rxBuf[2]);
// }

uint32_t ADS127L14_ReadSample(void) {
    uint8_t rxBuf[3];
    uint8_t txBuf[3] = {0,0,0}; // dummy transmit

    if (HAL_SPI_TransmitReceive(&hspi4, txBuf, rxBuf, 3, HAL_MAX_DELAY) == HAL_OK) {
        return ((uint32_t)rxBuf[0] << 16) |
               ((uint32_t)rxBuf[1] << 8) |
               ((uint32_t)rxBuf[2]);
    } else {
        return 0; // error case
    }
}



void MX_GPIO_Init(void) {
    //configure GPIO pins (RESET_ADS, START_ADS, MODE_ADS, CS_ADS, SPI4 pins)
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* RESET_ADS (PD0) */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* START_ADS (PD2) */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* MODE_ADS (PA3) */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CS_ADS (PA4) */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* SPI4 SCLK (PC2) */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* SPI4 MISO (PA7) */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* SPI4 MOSI (PB5) */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* Reset ADS127L14 */
void ADS127L14_Reset(void) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET); // RESET_ADS = PD0 low
    HAL_Delay(10);                                        // hold low for 10ms
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);   // RESET_ADS high
    HAL_Delay(10);                                        // wait after reset
}

/* Start ADS127L14 conversions */
void ADS127L14_Start(void) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);   // START_ADS = PD2 high
}

/* Configure ADS127L14 mode pins */
void ADS127L14_ConfigMode(void) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET); // MODE_ADS = PA3 (example: low = SPI mode)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // CS_ADS = PA4 low (chip select active)
}

