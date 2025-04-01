#ifndef PUZZLE
#define PUZZLE

#include <windows.h>
#include "inputs.h"

#define MAX_BYTES 1000

typedef struct
{
    POINT bitsPositions[16];
    POINT imageStart;
    POINT imageEnd;
    POINT currentAnalyserPos;
    int areaSize;
} ImageAnalyser;

void puzzle_start(HInputs *inputsHandle, POINT positions[18]);

void puzzle_analyseImage(ImageAnalyser *image);

void puzzle_printImages();

uint8_t apply_permutation(uint8_t byte, int perm[8]);

int next_permutation(int *a, int n);

int check_permutation(int perm[8], uint8_t *original, uint8_t *scrambled, int size);

#endif