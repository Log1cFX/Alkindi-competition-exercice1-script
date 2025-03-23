#include <stdint.h>
#include <windows.h>
#include "cursor.h"
#include "puzzle.h"

static uint8_t originalImage[40000] = {0};
static uint8_t currentImage[40000] = {0};

static void processPixel(POINT pos)
{

}

void puzzle_analyseImage(POINT positions[])
{
    // The rectangle is defined by positions[0] (top-left) and positions[1] (bottom-right)
    POINT imageStart = positions[0];
    POINT imageEnd = positions[1];

    POINT bitPixels[16];
    memcpy(bitPixels, &positions[2], 16 * sizeof(POINT));

    cursor_setPos(imageStart);

    // Iterate over each row from imageStart.y to imageEnd.y
    for (int y = imageStart.y; y <= imageEnd.y; y++)
    {

        if ((y - imageStart.y) % 2 == 0)
        {
            for (int x = imageStart.x; x < imageEnd.x; x++)
            {
                // Process the current pixel here.
                // For example: processPixelAt(x, y);
                // Move one pixel to the right.
                cursor_moveRight();
            }
        }
        else
        {
            for (int x = imageEnd.x; x > imageStart.x; x--)
            {
                // Process the current pixel here.
                // For example: processPixelAt(x, y);
                // Move one pixel to the left.
                cursor_moveLeft();
            }
        }

        // After finishing a row, if there is another row below, move the cursor down one pixel.
        if (y < imageEnd.y)
        {
            cursor_moveDown();
        }
    }
}