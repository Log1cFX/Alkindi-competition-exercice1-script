#include <stdio.h>
#include "inputs.h"
#include "cursor.h"
#include "screenshot.h"

#define maxPosNumber 18
POINT positions[maxPosNumber] = {0};
POINT x = {0};

int stopFunction(HInputs *HInputs, POINT *pos)
{
    for (int i = 0; i < maxPosNumber; i++)
    {
        if (positions[i].x == 0 && positions[i].y == 0)
        {
            positions[i] = *pos;
            printf("new pos added (%d,%d) %d out of %d\n", pos->x, pos->y, i + 1, maxPosNumber);
            free(pos);
            if (i + 1 == maxPosNumber)
            {
                return 1;
            }
            return 0;
        }
    }
    return 1;
}

int main()
{
    HInputs hInputs = {0};
    inputs_INIT(&hInputs, stopFunction, cursor_getPos);
    inputs_startListening(&hInputs);
    POINT *newPositions[16] = {0};
    for (int i = 2; i < 18; i++)
    {
        newPositions[i-2] = &(positions[i]);
    }
    byte *bytes = screenshot_capture(newPositions);
    printf("%d, %d", bytes[0], bytes[1]);
}