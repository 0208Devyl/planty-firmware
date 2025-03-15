#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"

void app_main(){
    gpio_set_direction(GPIO_NUM_15, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_15, GPIO_PULLDOWN_ONLY);

    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_23, 0);

    while(1){
        if(gpio_get_level(GPIO_NUM_15)){
            gpio_set_level(GPIO_NUM_23, 1);
        }else{
            gpio_set_level(GPIO_NUM_23, 0);
        }
        vTaskDelay(10);
    }
}