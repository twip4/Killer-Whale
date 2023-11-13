#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "LCD_Test.h"
#include "LCD_1in28.h"
#include <math.h>
#define PI 3.14159265358979323846
#include <unistd.h>

void drawClockIndexes(int xCenter, int yCenter, int radius, int color , float size, u_int8_t number);
void drawHand(int xCenter, int yCenter, int length, double angle, int color, int taille);
void drawClockHands(int xCenter, int yCenter, int hours, int minutes, int seconds, int color, int colorSec);
void drawPhisicalHour(int xCenter, int yCenter, int hours, int minutes, int seconds, int color, int size);
void drawClockNumber(u_int8_t xSize, u_int8_t ySize, u_int8_t radius, int color);

int Heure = 12 ;
int Minute = 5;
int Seconde = 0;
int Millisc = 0;

void Horloge(uint64_t last_time){
if (DEV_Module_Init() != 0)
    {
        return;
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

    drawClockIndexes(LCD_1IN28_HEIGHT/2, LCD_1IN28_HEIGHT/2, LCD_1IN28_HEIGHT/2,GRAY,0.95,60);
    drawClockIndexes(LCD_1IN28_HEIGHT/2, LCD_1IN28_HEIGHT/2, LCD_1IN28_HEIGHT/2,BLACK,0.9,12);


    LCD_1IN28_Display(BlackImage);

        while (true)
    {
        uint64_t current_time = to_us_since_boot(get_absolute_time());
        if (current_time - last_time >= 1000000) { // 1000000 microsecondes = 1000 millisecondes
            drawPhisicalHour(LCD_1IN28_HEIGHT/2,LCD_1IN28_HEIGHT/2,Heure,Minute,Seconde,BLACK,0);
            drawClockHands(LCD_1IN28_HEIGHT/2,LCD_1IN28_HEIGHT/2,Heure,Minute,Seconde,BLACK,RED);

            // drawClockNumber(LCD_1IN28_HEIGHT/2, LCD_1IN28_HEIGHT/2, LCD_1IN28_HEIGHT/2,BLACK);

            LCD_1IN28_Display(BlackImage);

            drawClockHands(LCD_1IN28_HEIGHT/2,LCD_1IN28_HEIGHT/2,Heure,Minute,Seconde,WHITE,WHITE);
            Seconde++;
            if (Seconde >= 60) {
                Seconde = 0;
                Minute = (Minute + 1) % 60; // Incrémente la minute, et revient à 0 après 59
            }

            if (Minute == 0) { // Augmente l'heure seulement si les minutes viennent de revenir à 0
                Heure = (Heure + 1) % 24; // Incrémente l'heure, et revient à 0 après 23
            }

            last_time = current_time; // Mettez à jour le dernier temps d'exécution
        }
    }
}

// void drawClockNumber(u_int8_t xSize, u_int8_t ySize, u_int8_t radius, int color){
//     int PosX[4] = {100,220,100,0};
//     int PosY[4] = {0,100,220,100};
//     for(int i = 0;i<=3;i++){
//         printf("x : %d, y : %d\n", PosX[i],PosY[i]);
//         char Cnumber = (char)i*3 ;
//         Paint_DrawChar(PosX[i],PosY[i],Cnumber,&Font24,BLACK,WHITE);
//     }
// }


void drawClockIndexes(int xCenter, int yCenter, int radius, int color , float size, u_int8_t number){
    for (int i = 0; i < number; i++) {
        // Convertir l'angle en radians
        double angle = i * (2 * PI / number);
        
        // Calculer le point de départ (à l'intérieur du cercle)
        int xStart = xCenter + (int)(radius * size * cos(angle));
        int yStart = yCenter + (int)(radius * size * sin(angle));
        
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

void drawClockHands(int xCenter, int yCenter, int hours, int minutes, int seconds, int color, int colorSec) {
    // Angle et longueur pour l'aiguille des heures
    double hourAngle = (hours % 12) * (2 * PI / 12) - (PI / 2);
    u_int8_t hourLength = 50; // La longueur de l'aiguille des heures
    drawHand(xCenter, yCenter, hourLength, hourAngle, color,3);

    // Angle et longueur pour l'aiguille des minutes
    double minuteAngle = minutes * (2 * PI / 60) - (PI / 2);
    u_int8_t minuteLength = 70; // La longueur de l'aiguille des minutes
    drawHand(xCenter, yCenter, minuteLength, minuteAngle, color,2);

    // Angle et longueur pour l'aiguille des secondes
    double secondAngle = seconds * (2 * PI / 60) - (PI / 2);
    u_int8_t secondLength = 90; // La longueur de l'aiguille des secondes
    drawHand(xCenter, yCenter, secondLength, secondAngle, colorSec,1);
    
    double antiSecondAngle = seconds * (2 * PI / 60) - (PI / 2);
    int8_t antiSecondLength = -20; // La longueur de l'aiguille des secondes
    drawHand(xCenter, yCenter, antiSecondLength, antiSecondAngle, colorSec,1);

    Paint_DrawCircle(xCenter, yCenter,3,colorSec,DOT_PIXEL_1X1,DRAW_FILL_FULL);
}

void drawPhisicalHour(int xStart, int yStart, int hours, int minutes, int seconds, int color, int size) {
    char Cheure[10];
    sprintf(Cheure, "%02d:%02d:%02d\0", hours,minutes,seconds);
    // printf("%s\n",Cheure);
    Paint_DrawString_EN(55,150,Cheure,&Font24,BLACK,WHITE);
}