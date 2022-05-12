#pragma once

#include <ncurses.h>
#include "libopz/opz_device.h"

void draw_mixer(WINDOW* _win, T3::opz_device * _opz) {
    T3::opz_project_data project = _opz->getProjectData();

    wclear(_win);
    box(_win, 0, 0);
    mvwprintw(_win, 1, 2, "DRUMS               SYNTH                PUNCH                MASTER");
    mvwprintw(_win, 2, 2, "%03i                 %03i                  %03i                  %03i", 
                            (int)((int)project.drum_level / 2.55f), 
                            (int)((int)project.synth_level / 2.55f), 
                            (int)((int)project.punch_level / 2.55f), 
                            (int)((int)project.master_level / 2.55f) );
    wrefresh(_win);
}