#include <Arduino.h>
#include "data_handling.h"
#include "hardware_config.h"

void get_sensor_data()
{
    Battery_Voltage = analogRead(Battery_Pin)*3/4095.;
    Moisture_Level = 2.1 * ( 0.7 - analogRead(Moisture_Pin)/4095. );

    // Measuring light levels and averaging the values
    digitalWrite(LightSens_Pins[1], HIGH);
    delay(10);
    Light_Level = analogRead(LightSens_Pins[0])/4095. - 0.2;
    delay(10);
    digitalWrite(LightSens_Pins[1], LOW);

    digitalWrite(Water_Pins[0], HIGH);
    delay(10);
    Tank_Level += digitalRead(Water_Pins[1]) + digitalRead(Water_Pins[2]);
    delay(10);
    digitalWrite(Water_Pins[0], LOW);

    Serial.println("Battery voltage: " + String(Battery_Voltage) + "V");
    Serial.println("Moisture level: " + String(Moisture_Level) + "%");
    Serial.println("Light level: " + String(Light_Level));
    Serial.println("Tank level: " + String(Tank_Level));
}