#include <stdint.h>
#include <windows.h>
#include "cursor.h"
#include "puzzle.h"
#include "screen.h"

uint8_t originalImage[40000] = {0};
uint8_t currentImage[40000] = {0};
static int index = 0;
static POINT bitPixels[16] = {0};
static HInputs *hInputs;

static int errorChecking(POINT pos)
{
    inputs_poll(hInputs);
}

static void processPixel(POINT pos)
{
    errorChecking(pos);
    uint8_t bytes[2] = {0};
    memcpy(bytes, screen_capturePixels(bitPixels), 2 * sizeof(uint8_t));
    if (index >= 40000)
    {
        return;
    }
    originalImage[index] = bytes[0];
    currentImage[index] = bytes[1];
    printf("byte number %d: (%d ; %d)\n", index, bytes[0], bytes[1]);
    index++;
    Sleep(1);

}

void puzzle_start(HInputs *inputsHandle, POINT positions[])
{
    hInputs = inputsHandle;
    puzzle_analyseImage(positions);
    // puzzle_printImages();
}

void puzzle_analyseImage(POINT positions[])
{
    // The rectangle is defined by positions[0] (top-left) and positions[1] (bottom-right)
    POINT imageStart = positions[0];
    POINT imageEnd = positions[1];

    memcpy(bitPixels, &positions[2], 16 * sizeof(POINT));

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
                cursor_moveRight();
            }
        }
        else
        {
            for (int x = imageEnd.x; x > imageStart.x; x--)
            {
                processPixel((POINT){x, y});
                cursor_moveLeft();
            }
        }

        // After finishing a row, if there is another row below, move the cursor down one pixel.
        if (y < imageEnd.y)
        {
            cursor_moveDown();
        }
    }
    cursor_leftUp();
}

void puzzle_printImages()
{
    for (int i = 0; i < 200; i++)
    {
        printf("byte number %d: (%d ; %d)\n", i, currentImage[i], originalImage[i]);
    }
}