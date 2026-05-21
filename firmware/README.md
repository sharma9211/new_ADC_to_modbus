# ADS127L14 to Modbus RTU Project

---

## 📌 Project Owner & Credits

- **Owner:** Rajnish  
- **Role:** Project creator and maintainer  
- **Contributions:**  
  - Designed and implemented STM32H563 firmware  
  - Integrated ADS127L14 external ADC via SPI  
  - Developed Modbus RTU slave communication over RS485  
  - Prepared documentation and testing flow for easy replication  

For queries, improvements, or collaboration, please reach out via GitHub Issues or Pull Requests.


## Overview
This project demonstrates how to interface the **ADS127L14 high‑precision ADC** with an **STM32H563 microcontroller** and expose the acquired data via **Modbus RTU** over RS485.  
The MCU acts as a Modbus slave, while a PC or PLC can query the ADC channel values as a Modbus master.

---

## Hardware Requirements
- STM32H563 MCU  
- ADS127L14 ADC  
- RS485 Transceiver (ISO3082)  
- Voltage Reference (ADR4540, 4.096 V)  
- Power supplies: +12V, +5V, +3.3V, +1.8V  

---

## 📌 How to Build
1. Prerequisites
Install ARM GNU Toolchain (e.g., arm-none-eabi-gcc, arm-none-eabi-objcopy)
Install Make (Windows ke liye MSYS2/MinGW ya Git Bash, Linux/Mac par default hota hai)
Optional: STM32CubeProgrammer (flashing ke liye)

2. Clone the Repository
git clone https://github.com/sharma9211/new_ADC_to_modbus.git
cd new_ADC_to_modbus/firmware

3. Clean Previous Builds
make clean

4. Build Firmware
make

Ye command generate karega:
firmware.elf → debugging ke liye
firmware.bin → raw binary flashing tools ke liye
firmware.hex → recommended for STM32CubeProgrammer

5. Flash to Board
STM32CubeProgrammer CLI (ST‑Link via SWD):
STM32_Programmer_CLI -c port=SWD -d firmware.hex -rst

STM32CubeProgrammer CLI (UART bootloader via COM port):
STM32_Programmer_CLI -c port=COM3 -d firmware.hex -rst // (COM port apne adapter ke hisab se set karo)

✅ Notes
Always run make clean before rebuilding if you change source files.
Use .hex file for production flashing.
Use .elf file for debugging with STM32CubeIDE or GDB/OpenOCD.

## Repository Structure
adc_modbus_project/
├── README.md
├── LICENSE
├── docs/
│   └── architecture.md
├── firmware/
│   ├── Core/Inc, Core/Src
│   ├── Drivers/
│   │   ├── adc_driver.c/.h
│   │   ├── modbus_slave.c/.h
│   │   └── rs485_driver.c
│   ├── App/
│   │   ├── main.c
│   │   ├── config.h
│   │   └── tasks.c
│   └── Makefile
├── test/
│   └── modbus_test.c
└── tools/
└── modbus_master.py


---

## Features
- SPI driver for ADC data acquisition  
- Modbus RTU slave stack implementation  
- RS485 driver for communication  
- Register mapping:  
  - 40001 → Channel 0  
  - 40002 → Channel 1  
  - 40003 → Channel 2  
  - 40004 → Channel 3  

---

## Getting Started
1. Clone the repository:
   ```bash
   git clone https://github.com/yourname/adc_modbus_project
2. Open in STM32CubeIDE.

3. Build and flash to the STM32H563 board.

4. Connect RS485 to PC/PLC master.

    Baud rate: 9600 (default)

    Slave ID: 1

    Parity: None, Stop bits: 1

5. Run a Modbus master tool (ModScan, Modbus Poll, or tools/modbus_master.py).

6. Read registers 40001–40004 to get ADC channel values.


from pymodbus.client import ModbusSerialClient

client = ModbusSerialClient(method='rtu', port='COM3', baudrate=9600)
client.connect()

result = client.read_holding_registers(0, 4, unit=1)
print("ADC Data:", result.registers)

client.close()
