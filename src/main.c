#include <stdio.h>
#include "pico/stdlib.h"
#include "Horloge.h"

uint8_t flag = 0;

const uint LED_PIN = 25 ;


int main(void){
    stdio_init_all();
    uint64_t last_time = to_us_since_boot(get_absolute_time());
    Horloge(last_time);
}