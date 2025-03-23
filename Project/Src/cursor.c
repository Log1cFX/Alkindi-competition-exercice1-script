#include "cursor.h"

// Move the mouse one pixel up
void cursor_moveUp()
{
    POINT pos;
    if (GetCursorPos(&pos))
    {
        SetCursorPos(pos.x, pos.y - 1);
    }
}

// Move the mouse one pixel down
void cursor_moveDown()
{
    POINT pos;
    if (GetCursorPos(&pos))
    {
        SetCursorPos(pos.x, pos.y + 1);
    }
}

// Move the mouse one pixel to the left
void cursor_moveLeft()
{
    POINT pos;
    if (GetCursorPos(&pos))
    {
        SetCursorPos(pos.x - 1, pos.y);
    }
}

// Move the mouse one pixel to the right
void cursor_moveRight()
{
    POINT pos;
    if (GetCursorPos(&pos))
    {
        SetCursorPos(pos.x + 1, pos.y);
    }
}

void cursor_leftDown()
{
    INPUT input[1] = {0};
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    if (SendInput(1, input, sizeof(INPUT)) == 0)
    {
        printf("SendInput failed: %lu\n", GetLastError());
    }
}

void cursor_leftUp()
{
    INPUT input[1] = {0};
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    if (SendInput(1, input, sizeof(INPUT)) == 0)
    {
        printf("SendInput failed: %lu\n", GetLastError());
    }
}

POINT *cursor_getPos()
{
    POINT *pos = malloc(sizeof(POINT));
    GetCursorPos(pos);
    return pos;
}