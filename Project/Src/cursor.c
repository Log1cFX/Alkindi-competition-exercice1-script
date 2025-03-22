#include "cursor.h"

// Move the mouse one pixel up
void cursor_moveUp() {
    POINT pos;
    if (GetCursorPos(&pos)) {
        SetCursorPos(pos.x, pos.y - 1);
    }
}

// Move the mouse one pixel down
void cursor_moveDown() {
    POINT pos;
    if (GetCursorPos(&pos)) {
        SetCursorPos(pos.x, pos.y + 1);
    }
}

// Move the mouse one pixel to the left
void cursor_moveLeft() {
    POINT pos;
    if (GetCursorPos(&pos)) {
        SetCursorPos(pos.x - 1, pos.y);
    }
}

// Move the mouse one pixel to the right
void cursor_moveRight() {
    POINT pos;
    if (GetCursorPos(&pos)) {
        SetCursorPos(pos.x + 1, pos.y);
    }
}