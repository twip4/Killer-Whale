#ifndef _MENU_H_
#define _MENU_H_

#include <stdio.h>
#include <unistd.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "LCD_1in28.h"
#include "CST816S.h"
#include "GUI_Paint.h"
#include "Images.h"

void Menu_display(uint16_t *Image);

#endif