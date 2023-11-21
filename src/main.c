#include <stdio.h>
#include "pico/stdlib.h"
#include "Menu.h"
#include "Horloge.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "Debug.h"
#include "CST816S.h"
#include "hardware/adc.h"
#include "QMI8658.h"

static void init_lcd();
static void Update_time();
void Touch_INT_callback(uint gpio, uint32_t events);

uint8_t flag = 0;
uint8_t reset = 1;

u_int32_t TabTime[3] = {15,30,0};
u_int32_t OldTabTime[3];

uint16_t *Image;

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
     // Initialiser le capteur tactile en mode point
    DEV_IRQ_SET(Touch_INT_PIN, GPIO_IRQ_EDGE_RISE, &Touch_INT_callback);
    //gpio_set_irq_enabled_with_callback(Touch_INT_PIN, GPIO_IRQ_EDGE_RISE, true, &Touch_INT_callback);

    // boucle temporelle
    while(true)
    {   
        //tight_loop_contents();
        uint64_t current_time = to_us_since_boot(get_absolute_time());
        // toute les secondes
        if (current_time - last_time >= 1000000) {
            Update_time();
            if (flag == 0){
                if (reset == 1){
                    // init horloge
                    printf("Horloge page \n");
                    Horloge_init(Image);
                    reset = 0;
                }
                Horloge_display(Image,TabTime,OldTabTime,1);
            }
            if (flag == 1){
                if (reset == 1){
                    printf("Menu page \n");
                    Menu_display(Image,1);
                    reset = 0;
                }
                Menu_display(Image,2);
                //printf("ok");
            }
            // printf("%02d:%02d:%02d\n", Heure,Minute,Seconde);
            last_time = current_time; 
        }      
    }

    DEV_Module_Exit();
    return 0;
}

static void init_lcd(){
    printf("LCD init \n");
    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    DEV_SET_PWM(100);
    uint32_t Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
    if ((Image = (uint16_t *)malloc(Imagesize)) == NULL)
    {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    Paint_NewImage((uint8_t *)Image,LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
    LCD_1IN28_Display(Image);
}

static void Update_time(){
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
}

void Touch_INT_callback(uint gpio, uint32_t events)
{
    if (gpio == Touch_INT_PIN)
    {
        if (Touch_CTS816.mode == CST816S_Gesture_Mode)
        {
            uint8_t gesture = CST816S_Get_Gesture();
            if(gesture == CST816S_Gesture_Long_Press){
                if (flag == 0){
                    flag = 1;
                    reset = 1;
                }
                else{
                    flag = 0;
                    reset = 1;
                }
                
            }
    }
}
}
