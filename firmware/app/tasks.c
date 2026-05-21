#include "tasks.h"
#include "adc_driver.h"
#include "modbus_slave.h"

void Update_ADC_Registers(void) {
    modbus_registers[0] = ADC_ReadChannel(0);
    modbus_registers[1] = ADC_ReadChannel(1);
    modbus_registers[2] = ADC_ReadChannel(2);
    modbus_registers[3] = ADC_ReadChannel(3);
}
