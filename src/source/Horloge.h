#ifndef _HORLOGE_H_
#define _HORLOGE_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "LCD_Test.h"
#include "LCD_1in28.h"
#include "QMI8658.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "CST816S.h"
#include <math.h>
#define PI 3.14159265358979323846
#include <unistd.h>

void Horloge(uint64_t last_time);

#endif