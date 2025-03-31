#ifndef PUZZLE
#define PUZZLE

#include <windows.h>
#include "inputs.h"

void puzzle_start(HInputs *inputsHandle, POINT positions[]);

void puzzle_analyseImage(POINT positions[]);

void puzzle_printImages();

void puzzle_calculateChances();

void puzzle_printProbabilites();

#endif