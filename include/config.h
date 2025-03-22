#define debug
#define use_display
#define use_moist_sens
#define use_light_sens
#define use_tank_sens
#define use_buttons

//=============================
// GPIO pin configuration
//=============================
int LightSens_Pins[3] = {12, 14, 27};   // Light Sensor Pins (photoresistors, all pins are inputs)
int Water_Pins[3] = {33, 25, 26};       // Water tank pins [3.3V OUTPUT, min lvl INPUT, max lvl INPUT]
int Moisture_Pin = 13;                  // Moisture Sensor Pin
int Battery_Pin = 34;                   // Battery Charge Pin

//=============================
// Display configuration
//=============================
#define hor_res         320
#define ver_res         240
// For pinout, see schematics in documentation folder
#define disp_spi_led    22
#define disp_spi_mosi   23
#define disp_spi_sclk   18
#define disp_spi_cs     5
#define disp_spi_rst    17
#define disp_spi_dc     16

//=============================
// Sleep-wake configuration
//=============================

#define wake_time       10 // Time [seconds] the device will be awake before sleeping again
#define sleep_time      10 // Time [seconds] the device will be asleep before waking again
