#include <stdint.h>
#include <windows.h>
#include "cursor.h"
#include "puzzle.h"
#include "screen.h"

static uint8_t originalImage[40000] = {0};
static uint8_t currentImage[40000] = {0};
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
    Sleep(100);
    errorChecking(pos);
    uint8_t bytes[2] = {0};
    uint8_t *temp = screen_capturePixels(bitPixels);
    memcpy(bytes, temp, 2 * sizeof(uint8_t));
    free(temp);
    if (index >= 40000)
    {
        return;
    }
    currentImage[index] = bytes[0];
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
    puzzle_calculateChances();
    puzzle_printProbabilites();
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
        printf("byte number %d: (" BYTE_TO_BINARY_PATTERN "," BYTE_TO_BINARY_PATTERN ")\n", i, BYTE_TO_BINARY(currentImage[i]), BYTE_TO_BINARY(originalImage[i]));
    }
}

static inline int count_ones(uint8_t x)
{
    return __builtin_popcount(x);
}

void puzzle_calculateChances()
{
    // iterates through all the pixels
    for (int pixelNum = 0; pixelNum < index; pixelNum++)
    {
        double weight = 0;
        int ones = count_ones(originalImage[pixelNum]);
        // avoid divisions by 0
        if (ones != 0)
        {
            weight = 100 / (double)ones;
        }

        // shifts bits of the currentImage one by one until finds a one
        for (int currentImageBitShift = 0; currentImageBitShift < 8; currentImageBitShift++)
        {
            // if finds a one
            if ((currentImage[pixelNum] >> currentImageBitShift) & 0X01)
            {
                // shifts bits of the originalImage one by one until finds a one
                for (int originalImageBitShift = 0; originalImageBitShift < 8; originalImageBitShift++)
                {
                    // if finds a one
                    if ((originalImage[pixelNum] >> originalImageBitShift) & 0x01)
                    {
                        // adds the weithed probablility of the real currentImages's bit
                        // number [currentImageBitShift] position being [originalImageBitShift]
                        bitPosProbability[currentImageBitShift][originalImageBitShift] += weight;
                    }
                }
            }
        }
    }
}

void puzzle_printProbabilites()
{
    for (int i = 0; i < 8; i++)
    {
        printf("for the bit %d of the current Image :\n", i + 1);
        for (int j = 0; j < 8; j++)
        {
            printf("- %d : %f\n", j + 1, bitPosProbability[i][j]);
        }
    }
}