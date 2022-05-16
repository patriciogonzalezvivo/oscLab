#pragma once

#include <math.h>
#include <ncurses.h>
#include "libopz/opz_device.h"

void draw_tempo(WINDOW* _win, T3::opz_device* _opz) {
    int lines, cols;
    getmaxyx(_win, lines, cols);

    T3::opz_project_data project = _opz->getProjectData();
    double pct = (_opz->getActiveStepId() % 8) / 8.0;
    
    werase(_win);
    box(_win, 0, 0);
    mvwprintw(_win, 1, 2,           "TEMPO               SWING");
    mvwprintw(_win, 2, 2,           "%03i                 %03i", project.tempo, (int)((int)project.swing / 2.55f) - 50);

    mvwprintw(_win, 1, cols - 28, "SOUND                LEVEL");
    mvwprintw(_win, 2, cols - 31, "%8s                  %03i", T3::metronomeSoundString(project.metronome_sound).c_str(), (int)((int)project.metronome_level / 2.55f) );

    size_t w = 12;
    mvwprintw(_win, 3, cols/2 - w, "        ####|####");
    mvwprintw(_win, 4, cols/2 - w, "        ####|####");
    mvwprintw(_win, 5, cols/2 - w, "       #####|#####");
    mvwprintw(_win, 6, cols/2 - w, "       #####|#####");
    mvwprintw(_win, 7, cols/2 - w, "      ######|######");
    mvwprintw(_win, 8, cols/2 - w, "      ######|######");
    mvwprintw(_win, 9, cols/2 - w, "     #######|#######");
    mvwprintw(_win,10, cols/2 - w, "     #######|#######");
    mvwprintw(_win,11, cols/2 - w, "    ########|########");
    mvwprintw(_win,12, cols/2 - w, "    ########|########");
    mvwprintw(_win,13, cols/2 - w, "   #########|#########");
    mvwprintw(_win,14, cols/2 - w, "   #########|#########");
    mvwprintw(_win,15, cols/2 - w, "  ##########|##########");
    mvwprintw(_win,16, cols/2 - w, "  ---------------------");
    mvwprintw(_win,17, cols/2 - w, " #######################");
    mvwprintw(_win,18, cols/2 - w, " ######### %03i #########", project.tempo);
    mvwprintw(_win,19, cols/2 - w, "#########################"); 

    wattron(_win, COLOR_PAIR(1));
    float x = w * sin( pct * 6.2831 );
    float y = 2 * abs( cos( pct * 6.2831 ) );
    dline(_win, cols/2,    16, 
                cols/2 + x, 5 - y,  '|');
    wattroff(_win, COLOR_PAIR(1));

    wrefresh(_win);
}
