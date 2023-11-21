#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "GUI_Paint.h"
#include "LCD_1in28.h"
#include <math.h>
#define PI 3.14159265358979323846
#include <unistd.h>

static void drawClockIndexes(int xCenter, int yCenter, int radius, int color , float size, u_int8_t number);
static void drawHand(int xCenter, int yCenter, int length, double angle, int color, int taille);
static void drawClockHands(int xCenter, int yCenter, int hours, int minutes, int seconds, int color, int colorSec);
static void drawPhisicalHour(int xCenter, int yCenter, int hours, int minutes, int seconds, int color, int size);
static void drawClockNumber(u_int8_t xSize, u_int8_t ySize, u_int8_t radius, int color);

// dessine les indexes autour du cadran
static void drawClockIndexes(int xCenter, int yCenter, int radius, int color , float size, u_int8_t number){
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
static void drawHand(int xCenter, int yCenter, int length, double angle, int color, int taille) {
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
    //printf("print %d %d %d %d", xCenter, yCenter, xEnd, yEnd);
    Paint_DrawLine(xCenter, yCenter, xEnd, yEnd, color,  pixelSize, LINE_STYLE_SOLID);
}

// dessine les aiguilles
static void drawClockHands(int xCenter, int yCenter, int hours, int minutes, int seconds, int color, int colorSec) {
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

// dessine l'heure en chiffre 
static void drawPhisicalHour(int xStart, int yStart, int hours, int minutes, int seconds, int color, int size) {
    char Cheure[10];
    //convertion int to str
    sprintf(Cheure, "%02d:%02d:%02d\0", hours,minutes,seconds);
    // printf("%s\n",Cheure);
    Paint_DrawString_EN(55,150,Cheure,&Font24,BLACK,WHITE);
}

// fonction d'init de l'affichage horloge 
void Horloge_init(uint16_t *Image){
        // /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_SetScale(65);
    Paint_Clear(WHITE);
    Paint_SetRotate(ROTATE_0);

    drawClockIndexes(LCD_1IN28_HEIGHT/2, LCD_1IN28_HEIGHT/2, LCD_1IN28_HEIGHT/2,GRAY,0.95,60);
    drawClockIndexes(LCD_1IN28_HEIGHT/2, LCD_1IN28_HEIGHT/2, LCD_1IN28_HEIGHT/2,BLACK,0.9,12);

    LCD_1IN28_Display(Image);
}

// focntion qui gere le dynamique de l'horloge 
void Horloge_display(uint16_t *Image,uint32_t *TabTime,uint32_t *OldTabTime,u_int8_t mode){

    // clear old clock hands
    drawClockHands(LCD_1IN28_HEIGHT/2,LCD_1IN28_HEIGHT/2,OldTabTime[0],OldTabTime[1],OldTabTime[2],WHITE,WHITE); 

    // mode avec que les aiguilles
    if(mode == 1){
        drawClockHands(LCD_1IN28_HEIGHT/2,LCD_1IN28_HEIGHT/2,TabTime[0],TabTime[1],TabTime[2],BLACK,RED);
    }

    // mode avec l'heure en chiffre en plus 
    if(mode == 2){
        drawClockHands(LCD_1IN28_HEIGHT/2,LCD_1IN28_HEIGHT/2,TabTime[0],TabTime[1],TabTime[2],BLACK,RED);
        drawPhisicalHour(LCD_1IN28_HEIGHT/2,LCD_1IN28_HEIGHT/2,TabTime[0],TabTime[1],TabTime[2],BLACK,0);
    }
    // draw number 
    Paint_DrawString_EN(205,110,"3",&Font24,BLACK,WHITE);
    Paint_DrawString_EN(113,200,"6",&Font24,BLACK,WHITE);
    Paint_DrawString_EN(20,110,"9",&Font24,BLACK,WHITE);
    Paint_DrawString_EN(104,20,"12",&Font24,BLACK,WHITE);

    LCD_1IN28_Display(Image);
}