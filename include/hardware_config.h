#pragma once

//=============================
// GPIO pin configuration
//=============================
extern const int LightSens_Pins[2];    // Light sensor pins [Analog read pin, Voltage supply pin]
extern const int Water_Pins[3]; // Water tank pins [3.3V OUTPUT, min lvl INPUT, max lvl INPUT]
extern const int Moisture_Pin;           
extern const int Battery_Pin;           
extern const int Button_Pins[3];   

//=============================
// Display configuration
//=============================
#define hor_res     320
#define ver_res     240
// For pinout, see schematics in documentation folder
#define LCD_LED     22
#define LCD_MOSI    23
#define LCD_SCLK    18
#define LCD_CS      5
#define LCD_RST     17
#define LCD_DC      16

//=============================
// Sleep-wake configuration
//=============================

#define wake_time   10 // Time [seconds] the device will be awake before sleeping again
#define sleep_time  10 // Time [seconds] the device will be asleep before waking again
