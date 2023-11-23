#include <stdio.h>
#include <unistd.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "LCD_1in28.h"
#include "CST816S.h"
#include "GUI_Paint.h"
#include "Images.h"
#include "TouchData.h"

void Menu_display(uint16_t *Image){

    Paint_Clear(WHITE);
    Paint_DrawImage1(logo_settings,(240-150)/2,(240-150)/2,150,150);
    LCD_1IN28_Display(Image);
    int8_t running = 1;
    while (running == 1)
    {
        if (CTouch.isTouched) {
            if (CTouch.gesture == CST816S_Gesture_Click){
            printf("click");
            running = 0;
            }
            CTouch.isTouched = false;
        }
    }
    

}


