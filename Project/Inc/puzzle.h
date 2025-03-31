#ifndef PUZZLE
#define PUZZLE

#include <windows.h>
#include "inputs.h"

void puzzle_start(HInputs *inputsHandle, POINT positions[]);

void puzzle_analyseImage(POINT positions[]);

void puzzle_printImages();

uint8_t apply_permutation(uint8_t byte, int perm[8]);

int next_permutation(int *a, int n);

int check_permutation(int perm[8], uint8_t *original, uint8_t *scrambled, int size);

#endif