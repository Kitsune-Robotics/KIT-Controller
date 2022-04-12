from discord_webhook import DiscordWebhook
from picamera import PiCamera
from time import sleep
from gps import *
from smbus import SMBus
import time

addr = 0x4 # bus address
bus = SMBus(1) # indicates /dev/ic2-1

numb = 1

def writeData(value):
    byteValue = StringToBytes(value)    
    bus.write_i2c_block_data(addr,0x00,byteValue) #first byte is 0=command byte.. just is.
    return -1


def StringToBytes(val):
        retVal = []
        for c in val:
                retVal.append(ord(c))
        return retVal

try:
    for _x in range (0, 2):
            for i in range(78, 130):
                    writeData("WIPE-" + str(i))
                    time.sleep(0.02)
            for i in range(130, 78, -1):
                    writeData("WIPE-" + str(i))
                    time.sleep(0.02)
except OSError:
    print("Could not speak to ardujmemo")

def get_uptime():
    with open('/proc/uptime', 'r') as f:
        uptime_seconds = float(f.readline().split()[0])

    return uptime_seconds

def getPositionData(gps):
    location = [None]
    while(location[0] == None):
        print("Trying again")
        nx = gpsd.next()
        # For a list of all supported classes and fields refer to:
        # https://gpsd.gitlab.io/gpsd/gpsd_json.html
        if nx['class'] == 'TPV':
            latitude = getattr(nx,'lat', "Unknown")
            longitude = getattr(nx,'lon', "Unknown")
            #print "Your position: lon = " + str(longitude) + ", lat = " + str(latitude)
            location = [latitude, longitude]
    return location

gpsd = gps(mode=WATCH_ENABLE|WATCH_NEWSTYLE)

loc = getPositionData(gpsd)

webhookURL = "https://discord.com/api/webhooks/856609966404534272/TR9tnLq2sIGZoOeADNswmGRNlzBcqM5aKihfU6snVTP9WhSSoVVvi7nT6i-ZfZS7Hcqm"

print(loc[0])
print(loc[1])
webhook = DiscordWebhook(url=webhookURL, content="Uptime: " + str( round( ((get_uptime() / 60) / 60 ), 2 )) + " hours. Lat is " + str(loc[0]) + ", long is " + str(loc[1]))

camera = PiCamera()
sleep(3) # let iso settle out
camera.capture('still.jpg')

with open("still.jpg", "rb") as f:
	webhook.add_file(file=f.read(), filename='still.jpg')
response = webhook.execute() # Hit send

