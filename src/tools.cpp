#include "tools.h"

#include <math.h>

#define sign(_x) (_x<0?-1:1)

std::string hBar(size_t _width, size_t _value) {
    std::string rta = "";
    size_t l = (_value/254.0) * _width;
    for (size_t i = 0; i < _width; i++)
        rta += (i < l ) ? "#" : ".";
    return rta;
}

void vBar(WINDOW* _win, int y, int x, size_t _height, size_t _value) {
    std::string rta = "";
    size_t l = (_value/254.0) * _height;
    for (size_t i = 0; i < _height; i++)
        mvwprintw(_win, y - i, x, "%s", (i < l ) ? "#" : ".");
}

/* Plot a point */
void plot(WINDOW* _win, int x, int y, int col) {
    mvwaddch(_win, y, x, (chtype) col);
}

/* Draw a diagonal(arbitrary) line using Bresenham's alogrithm. */
void dline(WINDOW* _win, int from_x, int from_y, int x2, int y2, int ch) {
    int dx, dy;
    int ax, ay;
    int sx, sy;
    int x, y;
    int d;

    dx = x2 - from_x;
    dy = y2 - from_y;

    ax = abs(dx * 2);
    ay = abs(dy * 2);

    sx = 
    sign(dx);
    sy = sign(dy);

    x = from_x;
    y = from_y;

    if (ax > ay) {
        d = ay - (ax / 2);

        while (1) {
            plot(_win, x, y, ch);
            if (x == x2)
                return;

            if (d >= 0) {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
        }
    } else {
        d = ax - (ay / 2);

        while (1) {
            plot(_win, x, y, ch);
            if (y == y2)
                return;

            if (d >= 0) {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
}