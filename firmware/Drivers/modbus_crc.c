#include "modbus_crc.h"

uint16_t Modbus_CRC16(const uint8_t *buf, uint16_t len) {
    uint16_t crc = 0xFFFF;

    for (uint16_t pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc

        for (int i = 0; i < 8; i++) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001; // Polynomial
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}
