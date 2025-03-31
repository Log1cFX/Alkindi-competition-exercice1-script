#include <stdint.h>
#include <windows.h>
#include "cursor.h"
#include "puzzle.h"
#include "screen.h"

static uint8_t originalImage[40000] = {0};
static uint8_t scrambledImage[40000] = {0};
static int index = 0;
static POINT bitPixels[16] = {0};
static HInputs *hInputs;
static double bitPosProbability[8][8] = {0};
static int skip = 0;

static void customFunction()
{
    skip = 1;
    printf("skip %d\n", skip);
}

static int errorChecking(POINT pos)
{
    inputs_poll(hInputs);
    return 0;
}

static void processPixel(POINT pos)
{
    Sleep(200);
    errorChecking(pos);
    uint8_t bytes[2] = {0};
    uint8_t *temp = screen_capturePixels(bitPixels);
    memcpy(bytes, temp, 2 * sizeof(uint8_t));
    free(temp);
    if (index >= 40000)
    {
        return;
    }
    scrambledImage[index] = bytes[0];
    originalImage[index] = bytes[1];
    index++;
}

void puzzle_start(HInputs *inputsHandle, POINT positions[18])
{
    hInputs = inputsHandle;
    hInputs->customFunction = customFunction;
    memcpy(bitPixels, &positions[2], 16 * sizeof(POINT));
    puzzle_analyseImage(positions);
    puzzle_printImages();

    int perm[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    do {
        if (check_permutation(perm, originalImage, scrambledImage, index)) {
            printf("Found matching permutation:\n");
            for (int i = 0; i < 8; ++i) {
                printf("%d ", perm[i]+1);
            }
            printf("\n");
            break;
        }
    } while (next_permutation(perm, 8));
    printf("didn't find any permutation\n");
}

void puzzle_analyseImage(POINT positions[])
{
    // The rectangle is defined by positions[0] (top-left) and positions[1] (bottom-right)
    POINT imageStart = positions[0];
    POINT imageEnd = positions[1];
    imageStart.x++;
    imageEnd.x++;

    cursor_setPos(imageStart);
    Sleep(1);
    cursor_leftDown();

    // Iterate over each row from imageStart.y to imageEnd.y
    for (int y = imageStart.y; y <= imageEnd.y; y++)
    {
        if ((y - imageStart.y) % 2 == 0)
        {
            for (int x = imageStart.x; x < imageEnd.x; x++)
            {
                processPixel((POINT){x, y});
                if (skip)
                {
                    return;
                }
                cursor_moveRight();
            }
        }
        else
        {
            for (int x = imageEnd.x; x > imageStart.x; x--)
            {
                // processPixel((POINT){x, y});
                cursor_moveLeft();
            }
        }

        // After finishing a row, if there is another row below, move the cursor down one pixel.
        if (y <= imageEnd.y)
        {
            // processPixel((POINT){0, 0});
            cursor_moveDown();
        }
    }
    cursor_leftUp();
}

void puzzle_printImages()
{
    for (int i = 0; i < 20; i++)
    {
        printf("byte number %d: (" BYTE_TO_BINARY_PATTERN "," BYTE_TO_BINARY_PATTERN ")\n", i, BYTE_TO_BINARY(scrambledImage[i]), BYTE_TO_BINARY(originalImage[i]));
    }
}

uint8_t apply_permutation(uint8_t byte, int perm[8]) {
    uint8_t result = 0;
    for (int i = 0; i < 8; ++i) {
        int from_bit = perm[i]; // where the i-th bit came from
        uint8_t bit = (byte >> from_bit) & 1;
        result |= (bit << i);
    }
    return result;
}

// Generate the next permutation (lexicographic order)
int next_permutation(int *a, int n) {
    int i = n - 2;h
    while (i >= 0 && a[i] > a[i + 1]) i--;
    if (i < 0) return 0;

    int j = n - 1;
    while (a[j] < a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;

    for (int k = i + 1, l = n - 1; k < l; k++, l--) {
        tmp = a[k]; a[k] = a[l]; a[l] = tmp;
    }
    return 1;
}

// Check if a permutation restores original from scrambled
int check_permutation(int perm[8], uint8_t *original, uint8_t *scrambled, int size) {
    for (int i = 0; i < size; ++i) {
        if (original[i] != apply_permutation(scrambled[i], perm)) {
            return 0;
        }
    }
    return 1;
}