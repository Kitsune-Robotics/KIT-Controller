#  Raspberry Pi Master for Arduino Slave
#  i2c_master_pi.py
#  Connects to Arduino via I2C
  
#  DroneBot Workshop 2019
#  https://dronebotworkshop.com

from smbus import SMBus
import time

addr = 0x8 # bus address
bus = SMBus(1) # indicates /dev/ic2-1

numb = 1

print ("Enter num")

for _x in range (0, 4):
	for i in range(76, 130):
		bus.write_byte(addr, i)
		time.sleep(0.02)
	for i in range(130, 76, -1):
                bus.write_byte(addr, i)
                time.sleep(0.02)

#while numb == 1:
#
#	ledstate = input(">>>>   ")
#
#	if ledstate == "1":
#		bus.write_byte(addr, 0x1) # switch it on
#	elif ledstate == "0":
#		bus.write_byte(addr, 0x0) # switch it on
#	else:
#		numb = 0
