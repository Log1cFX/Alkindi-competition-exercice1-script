#include <stdio.h>
#include "inputs.h"

int positions[5] = {0};
POINT x = {0};

int stopFunction(HInputs *HInputs, POINT *pos)
{
    for (int i = 0; i < 5; i++)
    {
        if (positions[i] == 0)
        {
            positions[i] = pos->x;
            printf("new pos added %d\n", pos->x);
            return 0;
        }
    }
    return 1;
}

POINT *getPos()
{
    x.x++;
    return &x;
}

int main()
{
    HInputs hInputs = {0};
    inputs_INIT(&hInputs, stopFunction, getPos);
    inputs_startListening(&hInputs);
}