#include "rs485_driver.h"
#include "usart.h"

void RS485_Send(uint8_t *data, uint16_t length) {
    HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET); // Enable driver
    HAL_UART_Transmit(&huart2, data, length, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET); // Disable driver
}
