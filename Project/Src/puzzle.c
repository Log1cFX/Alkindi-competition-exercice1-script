#include <stdint.h>
#include <windows.h>
#include "cursor.h"
#include "puzzle.h"
#include "screen.h"

static uint8_t originalImage[2][MAX_BYTES] = {0};  // [0][x] = even line, [1][x] = odd line
static uint8_t scrambledImage[2][MAX_BYTES] = {0}; // [0][x] = even line, [1][x] = odd line
static int pixelCount = 0; // total number of analyzed pixels
static int evenPixelCount = 0;
static int oddPixelCount = 0;
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
    int Xdif = image->imageEnd.x - image->imageStart.x;
    int Ydif = image->imageEnd.y - image->imageStart.y;
    int areaSize = Ydif * Xdif;
    if (areaSize == 0)
    {
        areaSize = Xdif;
    }
    else
    {
        areaSize += Xdif + Ydif;
    }
    areaSize++;
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
    initErrorCheck(&image);

    puzzle_analyseImage(&image);
    // puzzle_printImages();
    int permEven[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int permOdd[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    for (int odd = 0; odd < 2; odd++)
    {
        puzzle_getPermutaion((odd) ? permOdd : permEven, odd);
    }
    puzzle_printPermutations(permEven, permOdd);
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
    POINT a = image->currentAnalyserPos;
    POINT b = cursor_getPos();
    if (!(a.x == b.x && a.y == b.y))
    {
        printf("not fatal error found : unexpected cursor position at (%d,%d) instead of (%d,%d)\n", b.x, b.y, a.x, a.y);
        while (a.x != b.x && a.y != b.y)
        {
            printf("- cursor's position set at (%d,%d)\n", a.x, a.y);
            cursor_setPos(a);
            b = cursor_getPos();
        }
        printf("- skipping the pixel adding process\n");
        return 1;
    }
    return 0;
}
static void processPixel(ImageAnalyser *image)
{
    Sleep(200);
    if (!errorChecking(image))
    {
        uint8_t bytes[2] = {0};
        screen_capturePixels(bytes, image->bitsPositions);
        printf("byte number %d: (" BYTE_TO_BINARY_PATTERN "," BYTE_TO_BINARY_PATTERN ")\n", pixelCount, BYTE_TO_BINARY(bytes[0]), BYTE_TO_BINARY(bytes[1]));
        int odd = image->currentAnalyserPos.y % 2;
        scrambledImage[odd][pixelCount] = bytes[0];
        originalImage[odd][pixelCount] = bytes[1];
        pixelCount++;
        if (odd)
        {
            oddPixelCount++;
        }
        else
        {
            evenPixelCount++;
        }
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
        for (int x = imageStart.x; x <= imageEnd.x; x++)
        {
            image->currentAnalyserPos.x = x;
            cursor_setPos(image->currentAnalyserPos);
            processPixel(image);
            if (skip)
            {
                return;
            }
        }
    }
    cursor_leftUp();
}

void puzzle_printImages()
{
    for (int i = 0; i < 20; i++)
    {
        printf("byte number %d: (" BYTE_TO_BINARY_PATTERN "," BYTE_TO_BINARY_PATTERN ")\n",
               i, BYTE_TO_BINARY(scrambledImage[0][i]), BYTE_TO_BINARY(originalImage[0][i]));
    }
}

// puzzle_analyseImage functions END //

// permutations START //

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
static int check_permutation(int perm[8], uint8_t original[2][MAX_BYTES], uint8_t scrambled[2][MAX_BYTES], int odd)
{
    int size;
    if (odd)
    {
        size = oddPixelCount;
    }
    else
    {
        size = evenPixelCount;
    }
    for (int i = 0; i < size; ++i)
    {
        if (original[odd][i] != apply_permutation(scrambled[odd][i], perm))
        {
            return 0;
        }
    }
    return 1;
}

void puzzle_getPermutaion(int perm[8], int odd)
{
    while (next_permutation(perm, 8))
    {
        if (check_permutation(perm, originalImage, scrambledImage, odd))
        {
            printf("Found matching permutation\n");
            break;
        }
    }
}

void puzzle_printPermutations(int permA[8], int permB[8])
{
    for (int odd = 0; odd < 2; odd++)
    {
        int *var = (odd) ? permB : permA;
        printf("Pixel chiffre : ");
        for (int i = 7; i >= 0; i--)
        {
            printf("%d ", var[i] + 1);
        }
        printf("\n");
        printf("Resultat      : ");
        for (int i = 7; i >= 0; i--)
        {
            printf("%d ", i + 1);
        }
        printf("\n");
        printf("\n");
    }
}

// permutations END //