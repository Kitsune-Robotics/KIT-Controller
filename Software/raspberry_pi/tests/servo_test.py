import RPi.GPIO as GPIO
import time

servoPIN = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(servoPIN, GPIO.OUT)

p = GPIO.PWM(servoPIN, 50) # GPIO 17 for PWM with 50Hz
p.start(2.5) # Initialization
try:
  while True:
    p.ChangeDutyCycle(10)
    time.sleep(2)
    p.ChangeDutyCycle(2.5)
    time.sleep(2)
except KeyboardInterrupt:
  p.stop()
  GPIO.cleanup()

