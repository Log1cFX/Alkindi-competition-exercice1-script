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

void puzzle_getPermutaion(int perm[8], int odd);

void puzzle_printPermutations(int permA[8], int permB[8]);

#endif