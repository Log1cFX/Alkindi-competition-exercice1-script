#ifndef SCREEN
#define SCREEN

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)\
((byte) & 0x80 ? '1' : '0'),\
((byte) & 0x40 ? '1' : '0'),\
((byte) & 0x20 ? '1' : '0'),\
((byte) & 0x10 ? '1' : '0'),\
((byte) & 0x08 ? '1' : '0'),\
((byte) & 0x04 ? '1' : '0'),\
((byte) & 0x02 ? '1' : '0'),\
((byte) & 0x01 ? '1' : '0')

#include <windows.h>

void screen_capturePixels(uint8_t bytes[2], POINT bitsPositions[16]);

void screen_test(POINT positions[18]);

#endif