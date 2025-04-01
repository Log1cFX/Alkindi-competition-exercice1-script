#include <stdio.h>
#include <stdint.h>
#include "inputs.h"
#include "cursor.h"
#include "screen.h"
#include "puzzle.h"

#define maxPosNumber 18
POINT positions[maxPosNumber] = {0};
POINT x = {0};

static int stopFunction(HInputs *HInputs, POINT pos)
{
    for (int i = 0; i < maxPosNumber; i++)
    {
        if (positions[i].x == 0 && positions[i].y == 0)
        {
            positions[i] = pos;
            printf("new pos added (%d,%d) %d out of %d\n", pos.x, pos.y, i + 1, maxPosNumber);
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
    // printf(BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(0xE1));
    // int perm[8] = {2,3,4,8,5,7,1,6};
    // for (int i = 0; i < 8; i++)
    // {
    //     printf("%d ", perm[i]+1);
    // }
    // printf("\n");
    // printf(BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(apply_permutation(0x7F, perm)));
    HInputs hInputs = {0};
    hInputs.stopFunction = stopFunction;
    hInputs.customParamFunction = cursor_getPos;
    inputs_INIT(&hInputs);
    inputs_startListening(&hInputs);
    // screen_test(positions);
    puzzle_start(&hInputs, positions);
}