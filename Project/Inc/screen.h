#ifndef SCREEN
#define SCREEN

#include <windows.h>

uint8_t* screen_capturePixels(POINT positions[16]);

void screen_test(POINT positions[18]);

#endif