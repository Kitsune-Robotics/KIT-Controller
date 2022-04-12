/* Crawler Slave
 *  
 *  This code runs on the crawler i2c network
 *  and provides a cleaner, less CPU intensive control over PWM devices.
 */

#include <Wire.h>
#include <Servo.h>

// This servo is used to wipe and clean the camera lens
Servo windowWiperServo;

// Variables populated over i2c from master
int id;
int val;

void setup() {
  // For debugging
  //Serial.begin(115200);
  
  // Attach the wiper servo to pin 9
  windowWiperServo.attach(9);

  // This is the address the pi will speak to us at
  Wire.begin(0x4);
  
  // Call receiveEvent when data received                
  Wire.onReceive(receiveEvent);

  // Setup LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //Serial.println("Started");
}

// Just loop to keep the running code alive, and wait for events to happen.
void loop() {
  delay(50);
}

// This method runs when we receive a message
void receiveEvent(int n) {
  Wire.read(); // Remove smbus trash
  if (true) { // Dont do anything if this is not true
    id  = Wire.read(); // ID of the servo/device to access
    val = Wire.read(); // Value to assign

    //Serial.println(id);
    //Serial.println(val);
  
    switch(id) {
      case 1:
        windowWiperServo.write(val);
        break;
    }
  }

  // Prevents a bug where if bytes are left in buffer, arduino crashes.
  while (Wire.available()) {
    Wire.read();
  }
}
