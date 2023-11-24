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

    u_int8_t position = 0;
    u_int8_t positionMax = 4;
    u_int8_t click = 0;

    Paint_Clear(WHITE);
    Paint_DrawImage1(logo_horloge,(240-150)/2,(240-150)/2,150,150);
    LCD_1IN28_Display(Image);
    int8_t running = 1;
    while (running ==  1)
    {
        if (CTouch.isTouched) {
            if (CTouch.gesture == CST816S_Gesture_Click){
                click = 1;
            }
            if (CTouch.gesture == CST816S_Gesture_Left){
                // printf("gauche\n");
                if (position < positionMax){
                    position++;
                }
                // printf("position : %d\n",position);
                
            }
            if (CTouch.gesture == CST816S_Gesture_Right){
                // printf("droit\n");
                if (position > 0){
                    position--;
                }
                // printf("position : %d\n",position);
            }
            CTouch.isTouched = false;
        }

        switch (position) {
            case 0:
                if (click == 1){
                    running = 0;
                }
                Paint_DrawImage1(logo_horloge,(240-150)/2,(240-150)/2,150,150);
                LCD_1IN28_Display(Image);
                break;
            case 1:
                Paint_DrawImage1(logo_chrono,(240-150)/2,(240-150)/2,150,150);
                LCD_1IN28_Display(Image);
                break;
            case 2:
                Paint_DrawImage1(logo_minuteur,(240-150)/2,(240-150)/2,150,150);
                LCD_1IN28_Display(Image);
                break;
            case 3:
                Paint_DrawImage1(logo_alarme,(240-150)/2,(240-150)/2,150,150);
                LCD_1IN28_Display(Image);
                break;
            case 4:
                Paint_DrawImage1(logo_settings,(240-150)/2,(240-150)/2,150,150);
                LCD_1IN28_Display(Image);
                break;
            }
            printf("position : %d\n",position);
            click = 0;
        }
    }

