# A very simple makefile full of shortcuts

# What port the arduino is on
ifndef SERIAL_DEV
	ifneq (,$(wildcard /dev/ttyUSB0))
		SERIAL_DEV = /dev/ttyUSB0
	else ifneq (,$(wildcard /dev/ttyACM0))
		SERIAL_DEV = /dev/ttyACM0
	else
		SERIAL_DEV = unknown
	endif
endif

.PHONY: build test

ALL: build

build:
	pio run

flash:
	pio run -t upload

deep:
	pio run -t uploadeep

clean:
	pio run -t clean

lint:
	pio check

test:
	pio test

debug: flash monitor

monitor:
	pio device monitor
