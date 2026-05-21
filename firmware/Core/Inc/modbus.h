#ifndef MODBUS_H
#define MODBUS_H

#include "stm32h5xx_hal.h"

#define MODBUS_SLAVE_ID 1

extern uint16_t modbus_registers[10];

void Modbus_Init(UART_HandleTypeDef *huart);
void Modbus_Poll(void);
uint16_t Modbus_CRC16(uint8_t *buf, uint16_t len);   // <-- ADD THIS LINE

#endif
