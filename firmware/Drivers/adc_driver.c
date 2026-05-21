#include "adc_driver.h"

extern SPI_HandleTypeDef hspi1;

uint16_t ADC_ReadChannel(uint8_t channel) {
    uint16_t data = 0;
    // For ADS127L14, you typically read sequentially from DOUT pins
    HAL_SPI_Receive(&hspi1, (uint8_t*)&data, 2, HAL_MAX_DELAY);
    return data;
}
