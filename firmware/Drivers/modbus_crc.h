#ifndef MODBUS_CRC_H
#define MODBUS_CRC_H

#include <stdint.h>

uint16_t Modbus_CRC16(const uint8_t *buf, uint16_t len);

#endif
