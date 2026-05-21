CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-mcpu=cortex-m33 -mthumb -O2 -Wall \
 -ICore/Inc \
 -IDrivers/STM32H5xx_HAL_Driver/Inc \
 -IDrivers/CMSIS/Device/ST/STM32H5xx/Include \
 -IDrivers/CMSIS/Include \
 -DSTM32H563xx

LDFLAGS = -TSTM32H563ZITx_FLASH.ld

SRCS = \
 Core/Src/main.c \
 Core/Src/startup_stm32h563xx.s \
 Core/Src/system_stm32h5xx.c \
 Core/Src/stm32_assert.c \
 Core/Src/syscalls.c \
 Core/Src/modbus.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_adc.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_adc_ex.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.c \
 Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.c


OBJS=$(SRCS:.c=.o)

all: firmware.elf firmware.bin

firmware.elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f Core/Src/*.o Drivers/STM32H5xx_HAL_Driver/Src/*.o firmware.elf firmware.bin
