# Top-level Makefile for new_ADC_to_modbus project

.PHONY: all clean

# Default build: call firmware Makefile
all:
	$(MAKE) -C firmware all

# Clean build artifacts: call firmware Makefile
clean:
	$(MAKE) -C firmware clean
