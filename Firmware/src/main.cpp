#include <Arduino.h>
#include <PCD8544.h>

PCD8544 lcd = PCD8544(6, 7, 4, 8, 5);

// the setup routine runs once when you press reset:
void setup(void)
{
    // initialize the digital pin as an output.
    pinMode(16, OUTPUT);

    lcd.begin(84, 48);
}

// the loop routine runs over and over again forever:
void loop()
{
    digitalWrite(16, HIGH); // turn the LED on (HIGH is the voltage level)
    Serial.println("High");
    delay(1000);           // wait for a second
    digitalWrite(16, LOW); // turn the LED off by making the voltage LOW
    Serial.println("Low");
    delay(1000); // wait for a second

    lcd.setCursor(0, 0);
    lcd.print("   WELCOME  ");
    lcd.setCursor(0, 1);
    lcd.print("     To");
    lcd.setCursor(0, 2);
    lcd.print("ElectronicsHobbyists.com");
    delay(200);
}
