#include <stdio.h>
#include "pico/stdlib.h"
#include "Horloge.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "Debug.h"

static void init_lcd();
static void Update_time();

uint8_t flag = 0;

uint32_t Heure = 0 ;
uint32_t Minute = 59;
uint32_t Seconde = 50;

uint16_t *Image;

int main(void){
    // init time
    uint64_t last_time = to_us_since_boot(get_absolute_time());

    // init lib
    stdio_init_all();

    // init lcd
    init_lcd();

    // init horloge
    Horloge_init(Image);

    // boucle temporelle
    while(true)
    {
        uint64_t current_time = to_us_since_boot(get_absolute_time());
        // toute les secondes
        if (current_time - last_time >= 1000000) {
            Update_time();
            Horloge_display(Image,Heure,Minute,Seconde,2);
            last_time = current_time; 
        }
        
    }

    return 0;
}

static void init_lcd(){
    // init LCD
    if (DEV_Module_Init() != 0)
    {
        return ;
    }
    printf("Main\r\n");
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
}

static void Update_time(){
            Seconde++;
        if (Seconde >= 60) {
            Seconde = 0;
            Minute++; // Incrémente la minute, et revient à 0 après 59
        }
       if (Minute >= 60) {
            Minute = 0;
            Heure = (Heure + 1) % 24; 
        }
}