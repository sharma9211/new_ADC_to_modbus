#include "main.h"
#include "adc_driver.h"
#include "modbus_slave.h"
#include "tasks.h"

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART2_UART_Init();

    while (1) {
        Update_ADC_Registers();
        // UART interrupt will call Modbus_ProcessFrame when data arrives
    }
}
