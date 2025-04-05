#include <stdint.h>
#include <windows.h>
#include "cursor.h"
#include "puzzle.h"
#include "screen.h"

static uint16_t originalImage[2][MAX_BYTES] = {0};  // [0][x] = even line, [1][x] = odd line
static uint16_t scrambledImage[2][MAX_BYTES] = {0}; // [0][x] = even line, [1][x] = odd line
static int pixelCount = 0;                          // total number of analyzed pixels
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
}

void puzzle_start(HInputs *inputsHandle, POINT positions[22])
{
    printf("\n");
    printf("SOLVING THE PUZZLE\n");
    printf("\n");
    Sleep(1000);

    hInputs = inputsHandle;
    hInputs->customFunction = customFunction;

    ImageAnalyser sImage = {0};
    sImage.imageStart = positions[0];
    sImage.imageEnd = positions[1];
    memcpy(sImage.bitsPositions, &positions[4], 9 * sizeof(POINT));
    sImage.storedPixels = scrambledImage;

    ImageAnalyser oImage = {0};
    oImage.imageStart = positions[2];
    oImage.imageEnd = positions[3];
    memcpy(oImage.bitsPositions, &positions[13], 9 * sizeof(POINT));
    oImage.storedPixels = originalImage;

    initErrorCheck(&sImage);
    initErrorCheck(&oImage);

    for (int i = 0; i < 9; i++)
    {
        printf("(%d,%d)\n", sImage.bitsPositions[i].x, sImage.bitsPositions[i].y);
    }
    printf("\n");
    for (int i = 0; i < 9; i++)
    {
        printf("(%d,%d)\n", oImage.bitsPositions[i].x, oImage.bitsPositions[i].y);
    }
    

    POINT aa = sImage.imageStart;
    POINT ab = sImage.imageEnd;
    POINT ba = oImage.imageStart;
    POINT bb = oImage.imageEnd;

    if (!(ab.x - aa.x == bb.x - ba.x && ab.y - aa.y == bb.y - ba.y))
    {
        printf("FATAL ERROR : the areas don't have the same size\n");
        printf("- exiting program\n");
        exit(1);
    }

    printf("Analysing the scrambled Image\n");
    puzzle_analyseImage(&sImage);
    printf("Analysing the original Image\n");
    puzzle_analyseImage(&oImage);

    pixelCount = oImage.pixelCount;
    oddPixelCount = oImage.oddPixelCount;
    evenPixelCount = oImage.evenPixelCount;

    printf("Printing from array (scrambled)\n");
    puzzle_printImages(&sImage);
    printf("Printing from array (original)\n");
    puzzle_printImages(&oImage);

    int permEven[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int permOdd[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    for (int odd = 0; odd < 2; odd++)
    {
        puzzle_getPermutation((odd) ? permOdd : permEven, odd);
    }
    puzzle_printPermutations(permEven, permOdd);
}

// Initialization functions END //

// puzzle_analyseImage functions START //

static int errorChecking(ImageAnalyser *image)
{
    inputs_poll(hInputs);
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
    Sleep(100);
    if (!errorChecking(image))
    {
        uint16_t byte = {0};
        screen_capturePixels(&byte, image->bitsPositions);
        printf("byte number %d: "BYTE_TO_BINARY_PATTERN"\n", image->pixelCount, BYTE_TO_BINARY(byte));
        int odd = (image->currentAnalyserPos.y - image->imageStart.y) % 2;
        image->storedPixels[odd][(odd)? image->oddPixelCount : image->evenPixelCount] = byte;
        image->pixelCount++;
        if (odd)
        {
            image->oddPixelCount++;
        }
        else
        {
            image->evenPixelCount++;
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

void puzzle_printImages(ImageAnalyser *image)
{
    printf("%d\n", image->evenPixelCount);
    for (int i = 0; i < image->evenPixelCount; i++)
    {
        printf("byte number %d: " BYTE_TO_BINARY_PATTERN "\n" , i, BYTE_TO_BINARY(image->storedPixels[0][i]));
    }
}

// puzzle_analyseImage functions END //

// permutations START //

static uint16_t apply_permutation(uint16_t byte, int perm[9])
{
    uint16_t result = 0;
    for (int i = 0; i < 9; ++i)
    {
        int from_bit = perm[i]; // where the i-th bit came from
        uint16_t bit = (byte >> from_bit) & 1;
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
static int check_permutation(int perm[9], uint16_t original[2][MAX_BYTES], uint16_t scrambled[2][MAX_BYTES], int odd)
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

void puzzle_getPermutation(int perm[9], int odd)
{
    while (next_permutation(perm, 9))
    {
        if (check_permutation(perm, originalImage, scrambledImage, odd))
        {
            printf("Found matching permutation\n");
            break;
        }
    }
}

void puzzle_printPermutations(int permA[9], int permB[9])
{
    for (int odd = 0; odd < 2; odd++)
    {
        int *var = (odd) ? permB : permA;
        printf("Pixel chiffre : ");
        for (int i = 8; i >= 0; i--)
        {
            printf("%d ", var[i] + 1);
        }
        printf("\n");
        printf("Resultat      : ");
        for (int i = 8; i >= 0; i--)
        {
            printf("%d ", i + 1);
        }
        printf("\n");
        printf("\n");
    }
}

// permutations END //