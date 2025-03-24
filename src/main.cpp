// Library and framework includes 
#include <Arduino.h>
#include "driver/rtc_io.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc.h"
#define FS_NO_GLOBALS
//#include "FS.h"
//#include "SPIFFS.h"
//#include "SPI.h"
//#include "TFT_eSPI.h"
//#include "TJpg_Decoder.h"

// Project specific includes
#include "hardware_config.h"
#include "data_handling.h"
#include "lcd_gfx.h"

const int LightSens_Pins[2] = {12, 14};     // Light sensor pins [Analog read pin, Voltage supply pin]
const int Water_Pins[3]     = {33, 25, 26}; // Water tank pins [3.3V OUTPUT, min lvl INPUT, max lvl INPUT]
const int Moisture_Pin      = 13;           
const int Battery_Pin       = 34;           
const int Button_Pins[3]    = {2, 15, 4};  

float Battery_Voltage   = 0;
float Moisture_Level    = 0;
float Light_Level       = 0; 
int   Tank_Level        = 0; // 0 = empty, 1 = low, 2 = full

// Sleep interrupt configurations
#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO) // 2 ^ GPIO_NUMBER in hex
uint64_t bitmask = BUTTON_PIN_BITMASK(Button_Pins[0]) | BUTTON_PIN_BITMASK(Button_Pins[1]) | BUTTON_PIN_BITMASK(Button_Pins[2]);

uint32_t wakeup_time;

void print_GPIO_wake_up(){
    Serial.print("GPIO that triggered the wake up: GPIO ");
    Serial.println((log(esp_sleep_get_ext1_wakeup_status()))/log(2), 0);
  }

void print_wakeup_reason(){
    esp_sleep_wakeup_cause_t wakeup_reason;
  
    wakeup_reason = esp_sleep_get_wakeup_cause();
  
    switch(wakeup_reason)
    {
      case ESP_SLEEP_WAKEUP_EXT1: 
        Serial.println("Wakeup caused by external signal using RTC_CNTL"); 
        print_GPIO_wake_up();
        break;
      case ESP_SLEEP_WAKEUP_TIMER: 
        Serial.println("Wakeup caused by timer"); 
        break;
      default: 
        Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); 
        break;
    }
  }

void wakeup_routine(){
  Serial.begin(115200);
  delay(50); //Take some time to open up the Serial Monitor

  // Time recorded after waking up
  uint32_t wakeup_time = millis();

  pinMode(LCD_LED, OUTPUT);
  pinMode(Moisture_Pin, INPUT);
  pinMode(LightSens_Pins[0], INPUT);
  pinMode(LightSens_Pins[1], OUTPUT);
  pinMode(Water_Pins[0], OUTPUT);
  pinMode(Water_Pins[1], INPUT);
  pinMode(Water_Pins[2], INPUT);
  pinMode(Battery_Pin, INPUT);

  Battery_Voltage   = 0;
  Moisture_Level    = 0;
  Light_Level       = 0; 
  Tank_Level        = 0; // 0 = empty, 1 = low, 2 = full
}

void prepare_sleep(){
  esp_sleep_enable_timer_wakeup(sleep_time * 1e6);
  rtc_gpio_pullup_dis(gpio_num_t(Button_Pins[0]));
  rtc_gpio_pullup_dis(gpio_num_t(Button_Pins[1]));
  rtc_gpio_pullup_dis(gpio_num_t(Button_Pins[2]));
  rtc_gpio_pulldown_en(gpio_num_t(Button_Pins[0]));
  rtc_gpio_pulldown_en(gpio_num_t(Button_Pins[1]));
  rtc_gpio_pulldown_en(gpio_num_t(Button_Pins[2]));

  esp_sleep_enable_ext1_wakeup(bitmask, ESP_EXT1_WAKEUP_ANY_HIGH);

  gpio_hold_en(gpio_num_t(Button_Pins[0]));
  gpio_hold_en(gpio_num_t(Button_Pins[1]));
  gpio_hold_en(gpio_num_t(Button_Pins[2]));

  digitalWrite(LCD_LED, LOW);
  Serial.println("Going to sleep now");
  delay(100);
  Serial.flush(); 
}

void setup()
{
  wakeup_routine();
  print_wakeup_reason();
  prepare_lcd();

  get_sensor_data();
  display_notification();
}


void loop()
{
  if ( millis() - wakeup_time >= wake_time * 1e3 )
  {
    prepare_sleep();
    esp_deep_sleep_start();
  }
}