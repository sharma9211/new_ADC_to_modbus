#include "modbus_slave.h"
#include "modbus_crc.h"
#include "usart.h"

void Modbus_ProcessFrame(uint8_t *frame, uint16_t length) {
    uint8_t slave_id = frame[0];
    uint8_t func_code = frame[1];

    if (slave_id != MODBUS_SLAVE_ID) return;

    if (func_code == 0x03) { // Read Holding Registers
        uint16_t start_addr = (frame[2] << 8) | frame[3];
        uint16_t reg_count  = (frame[4] << 8) | frame[5];

        uint8_t response[64];
        response[0] = slave_id;
        response[1] = func_code;
        response[2] = reg_count * 2;

        for (int i = 0; i < reg_count; i++) {
            uint16_t val = modbus_registers[start_addr + i];
            response[3 + i*2] = val >> 8;
            response[4 + i*2] = val & 0xFF;
        }

        // Calculate CRC
        uint16_t crc = Modbus_CRC16(response, 3 + reg_count*2);
        response[3 + reg_count*2] = crc & 0xFF;       // CRC Low
        response[4 + reg_count*2] = (crc >> 8) & 0xFF; // CRC High

        Modbus_SendResponse(response, 5 + reg_count*2);
    }
}
