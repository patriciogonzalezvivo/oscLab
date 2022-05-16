#pragma once

#include <ncurses.h>
#include "libopz/opz_device.h"
#include "osc.h"

void draw_project(WINDOW* _win, T3::opz_device* _opz) {
    size_t project_id = _opz->getActiveProjectId();
    uint8_t pattern_id = _opz->getActivePatternId();
    T3::opz_pattern pattern = _opz->getActivePattern();
    T3::opz_track_id track_active = _opz->getActiveTrackId();

    int lines, cols;
    getmaxyx(_win, lines, cols);

    werase(_win);
    box(_win, 0, 0);

    mvwprintw(_win, 0, 2, " PROJECT  %02i ", project_id);
    mvwprintw(_win, 0, 20, " PATTERN  %02i ", pattern_id );

    // CHAINED PATTERNS (TODO)
    int song_width = 4;
    int x_margin = (cols - song_width * 16) / 2;
    for (size_t i = 0; i < 16; i++) {
        int y = 2;
        int x = x_margin + i * song_width;
        mvwprintw(_win, y, x, "%02X", _opz->getProjectData().pattern_chain[pattern_id].pattern[i]);
        mvwprintw(_win, y+1, x, "%02X", _opz->getProjectData().pattern_chain[pattern_id].pattern[i+16]);
    }

    // PATTERN TRACK
    int name_width = 10;
    int step_width = (cols - name_width) / 16;
    x_margin = 2 + (cols - step_width * 16 - name_width) / 2;
    size_t step_current = _opz->getActiveStepId();
    
    size_t tracks = 16;
    for (size_t t = 0; t < tracks; t++) {
        int y = 5 + t;
        if (t == track_active) wattron(_win, COLOR_PAIR(2));
        else if (t > 7) wattron(_win, COLOR_PAIR(4));
        mvwprintw(_win, y, x_margin, "%7s", T3::toString( T3::opz_track_id(t) ).c_str() );
        if (t == track_active) wattroff(_win, COLOR_PAIR(2));
        else if (t > 7) wattroff(_win, COLOR_PAIR(4));

        size_t step_count = _opz->getTrackParameters(T3::opz_track_id(t) ).step_count;
        size_t step_length = _opz->getTrackParameters(T3::opz_track_id(t) ).step_length;
        size_t step = (step_current / step_length) % step_count;

        // if (step_count > 0)
        //     send(trackUniformName(t) + "Pct", (float)step/(float)step_count);

        mvwprintw(_win,y, x_margin + name_width + step * step_width - 1, "[  ]");
        for (size_t s = 0; s < step_count; s++) {
            int x = x_margin + name_width + s * step_width;
            size_t i = _opz->getNoteIdOffset(t, s);

            if ( pattern.note[ i ].note == 0xFF) {
                if (t > 7) wattron(_win, COLOR_PAIR(4));
                mvwprintw(_win, y, x, "--");
                if (t > 7) wattroff(_win, COLOR_PAIR(4));
            }
            else {
                if (t == track_active) wattron(_win, COLOR_PAIR(1));
                else if (t > 7) wattron(_win, COLOR_PAIR(2));
                mvwprintw(_win, y, x, "%02X", pattern.note[ i ].note );
                if (t == track_active) wattroff(_win, COLOR_PAIR(1));
                else if (t > 7) wattroff(_win, COLOR_PAIR(2));
            }
        }
    }
    wrefresh(_win);
}