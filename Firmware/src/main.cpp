#include <Arduino.h>

// the setup routine runs once when you press reset:
void setup(void)
{
    // initialize the digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    Serial.println("High");
    delay(1000);                    // wait for a second
    digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
    Serial.println("Low");
    delay(1000); // wait for a second
}
