#include <stdint.h>
#include <windows.h>
#include "cursor.h"
#include "puzzle.h"
#include "screen.h"

static uint8_t originalImage[2][MAX_BYTES] = {0};  // [0][x] = even line, [1][x] = odd line
static uint8_t scrambledImage[2][MAX_BYTES] = {0}; // [0][x] = even line, [1][x] = odd line
static int pixelCount = 0;
static HInputs *hInputs;
static int skip = 0;

// Initialization functions START //

static void customFunction()
{
    skip = 1;
    printf("skip %d\n", skip);
}

static void initErrorCheck(ImageAnalyser *image)
{
    if ((image->imageStart.x > image->imageEnd.x) || (image->imageStart.y > image->imageEnd.y))
    {
        printf("FATAL ERROR : the first position isn't in the upper right corner\n");
        printf("- exiting program\n");
        exit(1);
    }
    int areaSize = (image->imageEnd.x - image->imageStart.x) * (image->imageEnd.y - image->imageStart.y);
    if (areaSize == 0)
    {
        areaSize = image->imageEnd.x - image->imageStart.x;
    }
    image->areaSize = areaSize;
    if (image->areaSize >= 1899)
    {
        printf("FATAL ERROR : the marked territory is too big\n");
        printf("- exiting program\n");
        exit(1);
    }
}

void puzzle_start(HInputs *inputsHandle, POINT positions[18])
{
    printf("\n");
    printf("SOLVING THE PUZZLE\n");
    printf("\n");
    Sleep(1000);

    hInputs = inputsHandle;
    hInputs->customFunction = customFunction;
    ImageAnalyser image = {0};
    // the bits are selected by the user and inserted from positions[2] to positions[18]
    memcpy(image.bitsPositions, &positions[2], 16 * sizeof(POINT));
    // The rectangle is defined by positions[0] (top-left) and positions[1] (bottom-right)
    image.imageStart = positions[0];
    image.imageEnd = positions[1];
    image.imageStart.x++;
    image.imageEnd.x++;
    initErrorCheck(&image);

    puzzle_analyseImage(&image);
    puzzle_printImages();
    int perm[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    puzzle_getPermutaion();
    // do
    // {
    //     if (check_permutation(perm, originalImage[][], scrambledImage[][], pixelCount))
    //     {
    //         printf("Found matching permutation:\n");
    //         for (int i = 0; i < 8; ++i)
    //         {
    //             printf("%d ", perm[i] + 1);
    //         }
    //         printf("\n");
    //         break;
    //     }
    // } while (next_permutation(perm, 8));
    // printf("didn't find any permutation\n");
}

// Initialization functions END //

// puzzle_analyseImage functions START //

static int errorChecking(ImageAnalyser *image)
{
    inputs_poll(hInputs);
    if (pixelCount >= image->areaSize)
    {
        printf("not fatal error found : pixelCount (%d) is bigger than the areaSize (%d)\n", pixelCount, image->areaSize);
        printf("- skipping the pixel adding process\n");
        return 1;
    }

    // POINT a = image->imageStart;
    // POINT b = image->imageEnd;
    // POINT c = cursor_getPos();
    // if (c.x < a.x || c.x > b.x || c.y < a.y || c.y > b.y)
    // {
    //     printf("FATAL ERROR : the cursor (%d, %d) is outside the bounds of the marked area\n", c.x, c.y);
    //     printf("- exiting program\n");
    //     exit(1);
    // }
    return 0;
}

static void processPixel(ImageAnalyser *image)
{
    Sleep(200);
    if (!errorChecking(image))
    {
        uint8_t bytes[2] = {0};
        screen_capturePixels(bytes, image->bitsPositions);
        int oddEven = image->currentAnalyserPos.y % 2;
        scrambledImage[oddEven][pixelCount] = bytes[0];
        originalImage[oddEven][pixelCount] = bytes[1];
        pixelCount++;
    }
}

void puzzle_analyseImage(ImageAnalyser *image)
{
    POINT imageStart = image->imageStart;
    POINT imageEnd = image->imageEnd;

    cursor_setPos(imageStart);
    Sleep(1);
    cursor_leftDown();

    // Iterate over each row from imageStart.y to imageEnd.y
    for (int y = imageStart.y; y <= imageEnd.y; y++)
    {
        image->currentAnalyserPos.y = y;
        if ((y - imageStart.y) % 2 == 0)
        {
            for (int x = imageStart.x; x < imageEnd.x; x++)
            {
                image->currentAnalyserPos.x = x;
                processPixel(image);
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
                image->currentAnalyserPos.x = x;
                processPixel(image);
                if (skip)
                {
                    return;
                }
                cursor_moveLeft();
            }
        }

        // After finishing a row, if there is another row below, move the cursor down one pixel.
        if (y <= imageEnd.y)
        {
            processPixel(image);
            cursor_moveDown();
        }
    }
    cursor_leftUp();
}

// puzzle_analyseImage functions END //

void puzzle_printImages()
{
    for (int i = 0; i < 20; i++)
    {
        printf("byte number %d: (" BYTE_TO_BINARY_PATTERN "," BYTE_TO_BINARY_PATTERN ")\n",
               i, BYTE_TO_BINARY(scrambledImage[0][i]), BYTE_TO_BINARY(originalImage[0][i]));
    }
}

static uint8_t apply_permutation(uint8_t byte, int perm[8])
{
    uint8_t result = 0;
    for (int i = 0; i < 8; ++i)
    {
        int from_bit = perm[i]; // where the i-th bit came from
        uint8_t bit = (byte >> from_bit) & 1;
        result |= (bit << i);
    }
    return result;
}

// Generate the next permutation (lexicographic order)
static int next_permutation(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] > a[i + 1])
        i--;
    if (i < 0)
        return 0;

    int j = n - 1;
    while (a[j] < a[i])
        j--;
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;

    for (int k = i + 1, l = n - 1; k < l; k++, l--)
    {
        tmp = a[k];
        a[k] = a[l];
        a[l] = tmp;
    }
    return 1;
}

// Check if a permutation restores original from scrambled
static int check_permutation(int perm[8], uint8_t *original, uint8_t *scrambled, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (original[i] != apply_permutation(scrambled[i], perm))
        {
            return 0;
        }
    }
    return 1;
}

puzzle_getPermutaion()
{
    
}