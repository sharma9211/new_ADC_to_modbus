# STM32H563 + ADS127L14 → Modbus RTU Project

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


## 📌 Overview
This project demonstrates how to interface an external high‑resolution ADC (ADS127L14) with an STM32H563 microcontroller and expose sampled data via Modbus RTU over RS485.  
The goal is to make ADC data accessible to any Modbus master tool (e.g., QModMaster, Modbus Poll).

---

## 📌 Block Diagram
┌───────────────┐       SPI4        ┌───────────────┐
│   STM32H563   │ <---------------->│   ADS127L14   │
│               │                   │ (External ADC)│
│               │                   └───────────────┘
│   UART3       │
│   (RS485)     │ <-----> RS485 Bus <-----> Modbus Master (PC Tool)
└───────────────┘


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

## 📌 System Components
- **STM32H563 MCU**: Runs HAL drivers, Modbus stack, and manages peripherals.
- **ADS127L14 ADC**: Provides 24‑bit high‑resolution samples via SPI4.
- **RS485 Transceiver**: Converts UART3 signals for Modbus RTU communication.
- **PC Master Tool**: Queries Modbus registers and displays ADC values.

---

## 📌 Firmware Flow
1. **SystemClock_Config**  
   - Configures HSE crystal + PLL for stable high‑speed operation (~250 MHz).
2. **Peripheral Init**  
   - GPIO: Reset, Start, Mode, CS pins for ADS127L14.  
   - SPI4: Master mode, CPHA=2EDGE, Polarity=Low.  
   - UART3: Configured for Modbus RTU (default 9600 baud, optional 115200).  
   - ADC1: Optional (if STM32 internal ADC is used).
3. **ADS127L14 Control**  
   - Reset, ConfigMode, Start functions manage ADC state.  
   - `ADS127L14_ReadSample()` reads 3 bytes via SPI and returns a 24‑bit sample.
4. **Modbus Integration**  
   - `Modbus_Init(&huart3)` initializes Modbus stack.  
   - Sample split into two 16‑bit registers:  
     - `modbus_registers[0]` → lower word  
     - `modbus_registers[1]` → upper word  
   - `Modbus_Poll()` handles master requests.
5. **Debugging**  
   - `printf("ADC Sample: %lu\n", sample);` prints raw sample for verification.

---

## 📌 Baud Rate Options
- **9600 baud** → Safe, reliable, recommended for initial testing and long RS485 cables.  
- **115200 baud** → High‑speed, suitable for continuous fast ADC streaming in clean environments.

---

## 📌 Testing Procedure
1. Install a Modbus master tool (QModMaster or Modbus Poll).  
2. Connect STM32 board via RS485 transceiver to PC.  
3. Configure tool:  
   - COM port = your RS485 adapter  
   - Baud rate = 9600 (or 115200 if configured)  
   - Slave ID = 1  
   - Function Code = 03 (Read Holding Registers)  
   - Starting Address = 0, Quantity = 2  
4. Send query → Response contains ADC sample split across two registers.  
5. Cross‑check with debug UART output.

---

## 📌 Example Modbus Response
01 03 04 XX XX YY YY CRC_L CRC_H


- `XX XX` = lower 16 bits of ADC sample  
- `YY YY` = upper 16 bits of ADC sample  
- `CRC_L CRC_H` = CRC16 checksum

---

## 📌 Conclusion
This project provides a complete pipeline:
- External ADC sampling via SPI  
- Modbus RTU slave communication via UART3 + RS485  
- Flexible baud rate (9600/115200)  
- Debugging via UART `printf` 

ADC_to_Modbus_Project/
│
├── Core/                # STM32 project source (CubeMX generated)
├── Drivers/             # HAL drivers
├── firmware/            # Build outputs (elf, bin)
│
├── docs/                # Documentation folder
│   ├── detailed_doc.md  # Full detailed documentation (step-by-step, diagrams)
│   └── adc_converter_new.pdf  # Your reference schematic/datasheet
│
├── README.md            # Quick Start summary (front page)
└── LICENSE              # Optional, choose MIT/BSD/GPL depending on sharing

Anyone can replicate and extend this project by following the steps above.
