// Library and framework includes 
#include <Arduino.h>
#include "driver/rtc_io.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc.h"

// Project specific includes
#include <config.h>

// Sleep interrupt configurations
#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO) // 2 ^ GPIO_NUMBER in hex
#define WAKEUP_GPIO_2              GPIO_NUM_2   // Only RTC IO are allowed - ESP32 Pin example
#define WAKEUP_GPIO_15             GPIO_NUM_15  // Only RTC IO are allowed - ESP32 Pin example
#define WAKEUP_GPIO_4              GPIO_NUM_4   // Only RTC IO are allowed - ESP32 Pin example
uint64_t bitmask = BUTTON_PIN_BITMASK(WAKEUP_GPIO_2) | BUTTON_PIN_BITMASK(WAKEUP_GPIO_15) | BUTTON_PIN_BITMASK(WAKEUP_GPIO_4);

void print_GPIO_wake_up(){
    int GPIO_reason = esp_sleep_get_ext1_wakeup_status();
    Serial.print("GPIO that triggered the wake up: GPIO ");
    Serial.println((log(GPIO_reason))/log(2), 0);
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
  

void setup(){
    Serial.begin(115200);
    delay(50); //Take some time to open up the Serial Monitor

    pinMode(disp_spi_led, OUTPUT);
    digitalWrite(disp_spi_led, HIGH);

    print_wakeup_reason();

    //Use ext1 as a wake-up source
    esp_sleep_enable_ext1_wakeup(bitmask, ESP_EXT1_WAKEUP_ANY_HIGH);
    // enable pull-down resistors and disable pull-up resistors

    delay(wake_time * 1e3);
    // Prep for sleep now
    esp_sleep_enable_timer_wakeup(sleep_time * 1e6);
    rtc_gpio_pullup_dis(GPIO_NUM_2);
    rtc_gpio_pullup_dis(GPIO_NUM_15);
    rtc_gpio_pullup_dis(GPIO_NUM_4);
    rtc_gpio_pulldown_en(GPIO_NUM_2);
    rtc_gpio_pulldown_en(GPIO_NUM_15);
    rtc_gpio_pulldown_en(GPIO_NUM_4);

    gpio_hold_en(GPIO_NUM_2);
    gpio_hold_en(GPIO_NUM_15);
    gpio_hold_en(GPIO_NUM_4);

    digitalWrite(disp_spi_led, LOW);
    Serial.println("Going to sleep now");
    delay(100);
    Serial.flush(); 
    // Sleeping now
    esp_deep_sleep_start();
}

void loop(){

}