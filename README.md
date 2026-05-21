# STM32H563 + ADS127L14 → Modbus RTU (RS485)

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
This project connects an external ADC (ADS127L14) to STM32H563 via SPI and exposes sampled data over Modbus RTU using RS485.  
PC master tools (QModMaster, Modbus Poll) can query ADC values directly.

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


---

## 📌 Block Diagram
STM32H563 ──SPI4──> ADS127L14 (ADC)
│
└─UART3──RS485──> Modbus Master (PC Tool)


---

## 📌 Setup
- **Clock:** HSE + PLL (~250 MHz)  
- **SPI4:** Master mode, CPHA=2EDGE, Polarity=Low  
- **UART3:** Modbus RTU, 9600 baud (safe) or 115200 baud (fast)  
- **GPIO:** Reset, Start, Mode, CS pins for ADC  

---

## 📌 Firmware Flow
1. Configure system clock (`SystemClock_Config`)  
2. Init peripherals (GPIO, SPI4, UART3)  
3. Read ADC sample via `ADS127L14_ReadSample()`  
4. Store sample in `modbus_registers[0]` and `[1]`  
5. Handle master requests with `Modbus_Poll()`  

---

## 📌 Testing
1. Install QModMaster / Modbus Poll  
2. Connect STM32 board via RS485 adapter  
3. Settings:  
   - COM port = adapter port  
   - Baud = 9600 (or 115200)  
   - Slave ID = 1  
   - Function Code = 03 (Read Holding Registers)  
   - Address = 0, Quantity = 2  
4. Query → Response contains ADC sample split across two registers  
5. Cross‑check with debug UART `printf` output  

---

## 📌 Example Response
01 03 04 XX XX YY YY CRC_L CRC_H

- `XX XX` = lower 16 bits of ADC sample  
- `YY YY` = upper 16 bits of ADC sample  

---

## 📌 Notes
- Use **9600 baud** for stable testing  
- Switch to **115200 baud** for high‑speed streaming  
- Debugging via UART `printf` helps verify values  

---

## ✅ Conclusion
This repo provides a complete pipeline:  
- External ADC sampling via SPI  
- Modbus RTU slave via UART3 + RS485  
- Flexible baud rate (9600/115200)  
- Easy testing with Modbus master tools

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
