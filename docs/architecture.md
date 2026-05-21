
---

## 📄 docs/architecture.md

```markdown
# System Architecture – ADS127L14 to Modbus RTU

## Purpose
The system converts high‑precision analog signals using the **ADS127L14 ADC**, processes them with the **STM32H563 MCU**, and exposes the data via **Modbus RTU** over RS485/CAN.

---

## Signal Flow
1. **Sensor Input → THS4551 Amplifier**  
   - Differential amplifier conditions the signal.  
   - RC filters reduce noise.  

2. **Amplifier → ADS127L14 ADC**  
   - ADC converts analog signals to 24‑bit digital data.  
   - Reference voltage (ADR4540, 4.096 V) ensures accuracy.  

3. **ADC → STM32H563 MCU (SPI)**  
   - MCU reads ADC data via SPI.  
   - Control pins (RESET, START, MODE) managed by MCU.  

4. **MCU → Modbus Registers**  
   - Data mapped into holding registers:  
     - 40001 → Channel 0  
     - 40002 → Channel 1  
     - 40003 → Channel 2  
     - 40004 → Channel 3  

5. **MCU → RS485/CAN Transceiver**  
   - RS485 (ISO3082) for Modbus RTU.  
   - CAN (ISO1044) for industrial bus communication.  

6. **Master (PC/PLC)**  
   - Queries the slave via Modbus.  
   - Receives ADC channel values.  

---

## Power Supply Architecture
- +12V main input.  
- Regulators generate:  
  - +5V (ADC, RS485, CAN)  
  - +3.3V (MCU, Flash)  
  - +1.8V (logic)  
- Isolated supplies for communication ICs.  

---

## Software Architecture
- **Drivers Layer**  
  - `adc_driver.c` → SPI read functions  
  - `rs485_driver.c` → UART + RS485 handling  
  - `modbus_slave.c` → Modbus RTU protocol stack  

- **Application Layer**  
  - `tasks.c` → Periodic ADC read + register update  
  - `main.c` → Initialization + infinite loop  

- **Tools Layer**  
  - Python script (`tools/modbus_master.py`) for testing  

---

## Data Flow Diagram
[Sensors] → [Amplifier] → [ADS127L14 ADC] → [STM32 MCU] → [Modbus Registers] → [RS485/CAN] → [Master PC/PLC]


---

## Summary
- Analog signals converted by ADS127L14.  
- MCU reads via SPI.  
- Data mapped into Modbus registers.  
- RS485/CAN transmits to master.  
- Master queries and retrieves values.
