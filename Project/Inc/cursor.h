#ifndef CURSOR
#define CURSOR

#include <windows.h>
#include <stdio.h>

void cursor_moveUp();

void cursor_moveDown();

void cursor_moveLeft();

void cursor_moveRight();

void cursor_setPos(POINT pos);

void cursor_leftDown();

void cursor_leftUp();

POINT cursor_getPos();

#endif