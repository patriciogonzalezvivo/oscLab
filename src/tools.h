#pragma once

#include <string>
#include <ncurses.h>

std::string hBar(size_t _width, size_t _value);

void vBar(WINDOW* _win, int y, int x, size_t _height, size_t _value);
void plot(WINDOW* _win, int x, int y, int col);
void dline(WINDOW* _win, int from_x, int from_y, int x2, int y2, int ch);