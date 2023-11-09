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

void drawClockIndexes(int xCenter, int yCenter, int radius, int color);
void drawHand(int xCenter, int yCenter, int length, double angle, int color, int taille);
void drawClockHands(int xCenter, int yCenter, int hours, int minutes, int seconds, int color);

uint8_t flag = 0;

const uint LED_PIN = 25 ;

int Heure = 12 ;
int Minute = 15;
int Seconde = 0;

int main(void){

if (DEV_Module_Init() != 0)
    {
        return -1;
    }
    printf("Main\r\n");
    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);

    DEV_SET_PWM(100);
    uint32_t Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
    uint16_t *BlackImage;
    if ((BlackImage = (uint16_t *)malloc(Imagesize)) == NULL)
    {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    // /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_NewImage((uint8_t *)BlackImage, LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
    Paint_SetScale(65);
    Paint_Clear(WHITE);
    Paint_SetRotate(ROTATE_0);
    Paint_Clear(WHITE);

    drawClockIndexes(240/2, 240/2, 240/2, BLACK);

    LCD_1IN28_Display(BlackImage);

        while (true)
    {
       drawClockHands(240/2,240/2,Heure,Minute,Seconde,BLACK);
       LCD_1IN28_Display(BlackImage);
       DEV_Delay_ms(1000);
       drawClockHands(240/2,240/2,Heure,Minute,Seconde,WHITE);
       Seconde++;
       if (Seconde >= 60)
       {
        Seconde = 0;
        Minute++;
       }
       if (Minute>= 60)
       {
        Minute = 0;
        Heure++;
       }
       if (Heure>=24)
       {
        Heure=0;
       }
    }
}


void drawClockIndexes(int xCenter, int yCenter, int radius, int color) {
    for (int i = 0; i < 12; i++) {
        // Convertir l'angle en radians
        double angle = i * (2 * PI / 12);
        
        // Calculer le point de départ (à l'intérieur du cercle)
        int xStart = xCenter + (int)(radius * 0.9 * cos(angle));
        int yStart = yCenter + (int)(radius * 0.9 * sin(angle));
        
        // Calculer le point de fin (sur le bord du cercle)
        int xEnd = xCenter + (int)(radius * cos(angle));
        int yEnd = yCenter + (int)(radius * sin(angle));
        
        // Dessiner l'index
        Paint_DrawLine(xStart, yStart, xEnd, yEnd, color, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    }

}

// Dessine l'aiguille, en spécifiant la longueur et l'angle
void drawHand(int xCenter, int yCenter, int length, double angle, int color, int taille) {
    int xEnd = xCenter + (int)(length * cos(-angle));
    int yEnd = yCenter - (int)(length * sin(-angle)); // Le système de coordonnées graphiques a l'axe Y inversé
    
    DOT_PIXEL pixelSize;
    switch(taille) {
        case 1: pixelSize = DOT_PIXEL_1X1; break;
        case 2: pixelSize = DOT_PIXEL_2X2; break;
        case 3: pixelSize = DOT_PIXEL_3X3; break;
        // Ajoutez d'autres cas si nécessaire
        default: pixelSize = DOT_PIXEL_1X1; // Valeur par défaut
    }
    Paint_DrawLine(xCenter, yCenter, xEnd, yEnd, color,  pixelSize, LINE_STYLE_SOLID);
}

void drawClockHands(int xCenter, int yCenter, int hours, int minutes, int seconds, int color) {
    // Angle et longueur pour l'aiguille des heures
    double hourAngle = ((hours % 12) + (minutes / 60.0)) * (2 * PI / 12) - (PI / 2);
    int hourLength = 50; // La longueur de l'aiguille des heures
    drawHand(xCenter, yCenter, hourLength, hourAngle, color,3);

    // Angle et longueur pour l'aiguille des minutes
    double minuteAngle = (minutes + (seconds / 60.0)) * (2 * PI / 60) - (PI / 2);
    int minuteLength = 70; // La longueur de l'aiguille des minutes
    drawHand(xCenter, yCenter, minuteLength, minuteAngle, color,2);

    // Angle et longueur pour l'aiguille des secondes
    double secondAngle = seconds * (2 * PI / 60) - (PI / 2);
    int secondLength = 90; // La longueur de l'aiguille des secondes
    drawHand(xCenter, yCenter, secondLength, secondAngle, color,1);
}