#ifndef MODBUS_SLAVE_H
#define MODBUS_SLAVE_H

#include "stm32h5xx_hal.h"

#define MODBUS_MAX_REGISTERS 10

extern uint16_t modbus_registers[MODBUS_MAX_REGISTERS];

void Modbus_ProcessFrame(uint8_t *frame, uint16_t length);
void Modbus_SendResponse(uint8_t *frame, uint16_t length);

#endif
