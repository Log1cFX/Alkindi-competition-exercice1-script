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

void screen_capturePixels(uint16_t *byte, POINT bitsPositions[9])
{
    HDC hdcScreen = GetDC(NULL);
    for (int i = 0; i < 9; i++)
    {
        int pixelColor = isBlack(getPixelColor(&hdcScreen, &bitsPositions[i]));
        *byte = (*byte | (pixelColor << i));
    }
    ReleaseDC(NULL, hdcScreen);
}