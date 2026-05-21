#include "modbus.h"

UART_HandleTypeDef *modbus_uart;
uint16_t modbus_registers[10];

void Modbus_Init(UART_HandleTypeDef *huart) {
    modbus_uart = huart;
}

void Modbus_Poll(void) {
    uint8_t rxBuf[8];
    if (HAL_UART_Receive(modbus_uart, rxBuf, sizeof(rxBuf), 10) == HAL_OK) {
        // Check slave ID
        if (rxBuf[0] != MODBUS_SLAVE_ID) return;

        // Function code (only 0x03 supported here: Read Holding Registers)
        if (rxBuf[1] == 0x03) {
            uint16_t startAddr = (rxBuf[2] << 8) | rxBuf[3];
            uint16_t numRegs   = (rxBuf[4] << 8) | rxBuf[5];

            uint8_t txBuf[64];
            txBuf[0] = MODBUS_SLAVE_ID;
            txBuf[1] = 0x03;
            txBuf[2] = numRegs * 2; // byte count

            for (uint16_t i = 0; i < numRegs; i++) {
                uint16_t val = modbus_registers[startAddr + i];
                txBuf[3 + i*2] = (val >> 8) & 0xFF;
                txBuf[4 + i*2] = val & 0xFF;
            }

            uint16_t crc = Modbus_CRC16(txBuf, 3 + numRegs*2);
            txBuf[3 + numRegs*2] = crc & 0xFF;       // CRC low
            txBuf[4 + numRegs*2] = (crc >> 8) & 0xFF; // CRC high

            HAL_UART_Transmit(modbus_uart, txBuf, 5 + numRegs*2, HAL_MAX_DELAY);
        }
    }
}

/* CRC16 calculation (Modbus RTU standard, polynomial 0xA001) */
uint16_t Modbus_CRC16(uint8_t *buf, uint16_t len) {
    uint16_t crc = 0xFFFF;
    for (uint16_t pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];
        for (int i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}
