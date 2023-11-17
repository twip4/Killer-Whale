#ifndef _HORLOGE_H_
#define _HORLOGE_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "LCD_1in28.h"
#include "QMI8658.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "CST816S.h"
#include <math.h>
#define PI 3.14159265358979323846
#include <unistd.h>

void Horloge_init(uint16_t *Image);
void Horloge_display(uint16_t *Image,uint32_t Heure,uint32_t Minute,uint32_t Seconde,u_int8_t mode);

#endif