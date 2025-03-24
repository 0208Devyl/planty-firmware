#include <Arduino.h>
#include "data_handling.h"
#include "hardware_config.h"

void get_sensor_data()
{
    Battery_Voltage = analogRead(Battery_Pin)*3/4095;
    Moisture_Level = analogRead(Moisture_Pin)/4095;

    // Measuring light levels and averaging the values
    Light_Level += analogRead(LightSens_Pins[0])/4095;
    Light_Level += analogRead(LightSens_Pins[1])/4095;
    Light_Level += analogRead(LightSens_Pins[2])/4095;
    Light_Level /= 3;

    digitalWrite(Water_Pins[0], HIGH);
    Tank_Level += digitalRead(Water_Pins[1]) + digitalRead(Water_Pins[2]);
    digitalWrite(Water_Pins[0], LOW);
}