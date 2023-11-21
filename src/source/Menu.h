#ifndef _MENU_H_
#define _MENU_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "LCD_1in28.h"
#include "CST816S.h"
#include "GUI_Paint.h"
#include <math.h>
#define PI 3.14159265358979323846
#include <unistd.h>

void Menu_display(uint16_t *Image,u_int8_t mode);

#endif