// TouchData.h
#ifndef TOUCH_DATA_H
#define TOUCH_DATA_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t gesture;
    bool isTouched;
} TouchData;

extern volatile TouchData CTouch;

#endif
