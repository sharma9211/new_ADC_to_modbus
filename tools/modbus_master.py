from pymodbus.client import ModbusSerialClient

def main():
    # Configure Modbus RTU client
    client = ModbusSerialClient(
        method='rtu',
        port='COM3',       # Change to your serial port (e.g., COM3 on Windows, /dev/ttyUSB0 on Linux)
        baudrate=9600,
        parity='N',
        stopbits=1,
        bytesize=8,
        timeout=2
    )

    if not client.connect():
        print("❌ Failed to connect to Modbus slave")
        return

    print("✅ Connected to Modbus slave")

    # Read 4 registers starting at address 0 (40001–40004)
    result = client.read_holding_registers(address=0, count=4, unit=1)

    if result.isError():
        print("❌ Error reading registers:", result)
    else:
        print("📊 ADC Channel Data:")
        for i, val in enumerate(result.registers):
            print(f"Channel {i}: {val}")

    client.close()

if __name__ == "__main__":
    main()
