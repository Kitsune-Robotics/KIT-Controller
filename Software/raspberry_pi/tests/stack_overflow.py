import smbus
import time
import struct

# for RPI version 1, use bus = smbus.SMBus(0)
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

try:
    for _x in range (0, 2):
            for i in range(78, 130):
                    bus.write_i2c_block_data(address, 0, [1, i])
                    time.sleep(0.02)
            for i in range(130, 78, -1):
                    bus.write_i2c_block_data(address, 0, [1, i])
                    time.sleep(0.02)
except OSError:
    print("Could not speak to ardujmemo")
