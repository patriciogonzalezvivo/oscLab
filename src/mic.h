#pragma once

#include <ncurses.h>
#include "libopz/opz_device.h"

void draw_mic(WINDOW* _win, T3::opz_device* _opz) {
    werase(_win);
    box(_win, 0, 0);
    mvwprintw(_win, 1, 2, "MIC LEVEL                  MIC FX ");
    mvwprintw(_win, 2, 2, "%s                  %s", 
                            hBar(9, _opz->getMicLevel() ).c_str(), 
                            T3::toString(_opz->getMicFx()).c_str());
    wrefresh(_win);
}