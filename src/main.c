#include <stdio.h>
#include "pico/stdlib.h"
#include "Menu.h"
#include "Horloge.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "CST816S.h"
#include "hardware/adc.h"
#include "QMI8658.h"
#include "TouchData.h"

void init_lcd();
bool Update_time(struct repeating_timer *t);
void Touch_INT_callback(uint gpio, uint32_t events);

uint8_t flag = 0;
uint8_t reset = 1;

u_int32_t TabTime[3] = {15,30,0};
u_int32_t OldTabTime[3];

uint16_t *Image;

uint32_t Imagesize;

int main(void){

    // init lib
    stdio_init_all();

    if (DEV_Module_Init() != 0)
    {
        printf("Module initialization failed.\r\n");
        return -1;
    }

    // init lcd
    init_lcd();

    // init time
    uint64_t last_time = to_us_since_boot(get_absolute_time());
    
    // init touch
    CST816S_init(CST816S_Gesture_Mode);
    DEV_KEY_Config(Touch_INT_PIN);
    DEV_IRQ_SET(Touch_INT_PIN, GPIO_IRQ_EDGE_RISE, &Touch_INT_callback);

    // init callback fonction for update time in background
    repeating_timer_t timer;
    bool success = add_repeating_timer_ms(1000, Update_time, NULL, &timer);
    if (success) {
        printf("callback time init !!! \n");
    }
    // boucle temporelle
    while(true)
    {   
        if (CTouch.isTouched) {
            if (CTouch.gesture == CST816S_Gesture_Long_Press){
                if (flag == 1){
                    flag = 0;
                    reset = 1;
                }
                else{
                    flag = 1;
                }
            }
            CTouch.isTouched = false;
        }

        if (flag == 0){
            if (reset == 1){
                    printf("Horloge page \n");
                    Horloge_init(Image);
                    reset = 0;
                }
                Horloge_display(Image,TabTime,OldTabTime,2);
            }
            if (flag == 1){
                printf("Menu page \n");
                Menu_display(Image);
                flag = 0;
                reset = 1;
            }     
        }

    DEV_Module_Exit();
    return -1;
}

void init_lcd(){
    printf("LCD init \n");
    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    DEV_SET_PWM(100);
    Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
    if ((Image = (uint16_t *)malloc(Imagesize)) == NULL)
    {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    Paint_NewImage((uint8_t *)Image,LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
    Paint_SetRotate(ROTATE_180);
    LCD_1IN28_Display(Image);
}

bool Update_time(struct repeating_timer *t){

    for(int i;i<3;i++){
        OldTabTime[i] = TabTime[i];
    }
            TabTime[2]++;
        if (TabTime[2] >= 60) {
            TabTime[2] = 0;
            TabTime[1]++; // Incrémente la minute, et revient à 0 après 59
        }
       if (TabTime[1] >= 60) {
           TabTime[1] = 0;
           TabTime[0] = (TabTime[0] + 1) % 24; 
        }
    // debug time
    // printf("%d:%d:%d\n",TabTime[0],TabTime[1],TabTime[2]);
    return true;
}

void Touch_INT_callback(uint gpio, uint32_t events){
    CST816S_Get_Point();
    uint8_t gesture = CST816S_Get_Gesture();
    CTouch.x = Touch_CTS816.x_point;
    CTouch.y = Touch_CTS816.y_point;
    CTouch.gesture = gesture;
    CTouch.isTouched = true;
}