#ifndef PUZZLE
#define PUZZLE

#include <windows.h>
#include "inputs.h"

#define MAX_BYTES 1000

typedef struct
{
    uint16_t (*storedPixels)[MAX_BYTES];
    POINT bitsPositions[9];
    POINT imageStart;
    POINT imageEnd;
    POINT currentAnalyserPos;
    int areaSize;
    int pixelCount;
    int oddPixelCount;
    int evenPixelCount;
} ImageAnalyser;

void puzzle_start(HInputs *inputsHandle, POINT positions[22]);

void puzzle_analyseImage(ImageAnalyser *image);

void puzzle_printImages(ImageAnalyser *image);

void puzzle_getPermutation(int perm[9], int odd);

void puzzle_printPermutations(int permA[9], int permB[9]);

#endif