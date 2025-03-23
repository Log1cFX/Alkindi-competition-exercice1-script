#include <windows.h>
#include <stdint.h>
#include "screenshot.h"

static byte returnValue[2] = {0};

static COLORREF getPixelColor(HDC *hdcScreen, POINT *pos)
{
    return GetPixel(*hdcScreen, pos->x, pos->y);
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
    return (brightness < 128.0);
}

// REMEMBER TO COPY VALUES FROM THE ARRAY AND NOT USE THE POINTER
byte* screenshot_capture(POINT *positions[16])
{
    HDC hdcScreen = GetDC(NULL);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int pixelColor = isBlack(getPixelColor(&hdcScreen, positions[(8*i)+j]));
            returnValue[i] = (returnValue[i]|(pixelColor<<j));
        }
    }
    ReleaseDC(NULL, hdcScreen);
    return returnValue;
}