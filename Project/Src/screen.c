#include <windows.h>
#include <stdint.h>
#include "screen.h"
#include <stdio.h>

static int isBlack(COLORREF color);

static COLORREF getPixelColor(HDC *hdcScreen, POINT *pos)
{
    // isBlack(GetPixel(*hdcScreen, (int)pos->x, (int)pos->y));
    return GetPixel(*hdcScreen, (int)pos->x, (int)pos->y);
}

static int isBlack(COLORREF color)
{
    // Extract red, green, and blue components
    int red = GetRValue(color);
    int green = GetGValue(color);
    int blue = GetBValue(color);

    // Compute brightness using the luminance formula
    double brightness = 0.299 * red + 0.587 * green + 0.114 * blue;

    // Define threshold: colors with brightness less than 128 are considered close to black
    return (brightness < 200.0);
}

// REMEMBER TO COPY VALUES FROM THE ARRAY AND NOT USE THE POINTER
uint8_t *screen_capturePixels(POINT positions[16])
{
    HDC hdcScreen = GetDC(NULL);
    uint8_t *returnValue = malloc(2 * sizeof(uint8_t));
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int pixelColor = isBlack(getPixelColor(&hdcScreen, &positions[(8 * i) + j]));
            returnValue[i] = (returnValue[i] | (pixelColor << j));
        }
    }
    ReleaseDC(NULL, hdcScreen);
    return returnValue;
}

void screen_test(POINT positions[18])
{
    for (int i = 2; i < 18; i++)
    {   
        HDC hdcScreen = GetDC(NULL);
        printf("%d", isBlack(getPixelColor(&hdcScreen, &positions[i])));
        ReleaseDC(NULL, hdcScreen);
    }
}