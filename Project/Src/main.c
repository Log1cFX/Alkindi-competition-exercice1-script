#include <stdio.h>
#include <stdint.h>
#include "inputs.h"
#include "cursor.h"
#include "screen.h"
#include "puzzle.h"

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
    Sleep(2000);
    // screen_test(positions);
    puzzle_start(&hInputs, positions);
}