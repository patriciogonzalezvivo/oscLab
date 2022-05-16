#pragma once

#include <ncurses.h>
#include "libopz/opz_device.h"
#include "tools.h"

// PAGE 1: SOUND     
void draw_page_one(WINDOW* _win, T3::opz_device* _opz) {
    mvwprintw(_win, 1, 1, "SOUND  P1      P2      FILTER  RESONA.");
    wattron(_win, COLOR_PAIR(4));
    mvwprintw(_win, 2, 1, "       %s %s %s %s",   hBar(7, (size_t)_opz->getActivePageParameters().param1).c_str(),
                                            hBar(7, (size_t)_opz->getActivePageParameters().param2).c_str(),
                                            hBar(7, (size_t)_opz->getActivePageParameters().filter).c_str(),
                                            hBar(7, (size_t)_opz->getActivePageParameters().resonance).c_str() );
    wattroff(_win, COLOR_PAIR(4));
    mvwprintw(_win, 3, 1, "       %7i %7i %7i %7i", 
                                            (int)((int)_opz->getActivePageParameters().param1 / 2.55f), 
                                            (int)((int)_opz->getActivePageParameters().param2 / 2.55f), 
                                            (int)((int)_opz->getActivePageParameters().filter / 2.55f), 
                                            (int)((int)_opz->getActivePageParameters().resonance / 2.55f) );
}

// PAGE 2: ENVELOPE
void draw_page_two(WINDOW* _win, T3::opz_device* _opz) {
    size_t track_id = _opz->getActiveTrackId();
    size_t page_id = _opz->getActivePageId();

    werase(_win);

    if (page_id == 1) wattron(_win, COLOR_PAIR(1));
    box(_win, 0, 0);
    wattroff(_win, COLOR_PAIR(1));

    mvwprintw(_win, 1, 1, "ENVEL.");

    float attack = (int)_opz->getActivePageParameters().attack / 255.0f;
    float decay = (int)_opz->getActivePageParameters().decay / 255.0f;
    float sustain = (int)_opz->getActivePageParameters().sustain / 255.0f;
    float release = (int)_opz->getActivePageParameters().release / 255.0f;
    size_t x = 8;
    size_t w = 30;

    if (track_id < 4) {
        // TODO:
        //  - draw envelope
        // ┌───────────────────────────────────────┐
        // │ENVEL.                                 │
        // │                                       │
        // │                                       │
        // │                                       │
        // │                                       │
        // │       S 0     A 0     H 0     D 0     │
        // └───────────────────────────────────────┘
        size_t s = attack * w;
        size_t a = s + ( (w-s)/2 ) * decay;
        size_t h = a + ( (w-a) * sustain);
        size_t d = h + ( (w-h) * release);

        s += x;
        a += x;
        h += x;
        d += x;

        wattron(_win, COLOR_PAIR(4));
        dline(_win, s, 5, a, 1, '.');
        dline(_win, a, 1, h, 1, '.');
        dline(_win, h, 1, d, 5, '.');
        wattroff(_win, COLOR_PAIR(4));

        wattron(_win, COLOR_PAIR(2));
        mvwprintw(_win, 5, s, "+");
        mvwprintw(_win, 1, a, "+");
        mvwprintw(_win, 1, h, "+");
        mvwprintw(_win, 5, d, "+");
        wattroff(_win, COLOR_PAIR(2));

        mvwprintw(_win, 6,  8, "S %i", (int)(100*attack));
        mvwprintw(_win, 6, 17, "A %i", (int)(100*decay));
        mvwprintw(_win, 6, 25, "H %i", (int)(100*sustain));
        mvwprintw(_win, 6, 34, "D %i", (int)(100*release));
    }
    else {

        size_t a = attack * (w/4);
        size_t d = a + (w/4-a) * decay;
        size_t h = 5 - 4 * sustain;
        size_t r = d + ( (w-d) * release);

        a += x;
        d += x;
        r += x;

        wattron(_win, COLOR_PAIR(4));
        dline(_win, x, 5, a, 1, '.');
        dline(_win, a, 1, d, h, '.');
        dline(_win, d, h, r, h, '.');
        dline(_win, r, h, x+w, 5, '.');
        wattroff(_win, COLOR_PAIR(4));

        wattron(_win, COLOR_PAIR(2));
        mvwprintw(_win, 1, a, "+");
        mvwprintw(_win, h, d, "+");
        mvwprintw(_win, h, r, "+");
        wattroff(_win, COLOR_PAIR(2));

        mvwprintw(_win, 6,  8, "A %i", (int)(100*attack));
        mvwprintw(_win, 6, 17, "D %i", (int)(100*decay));
        mvwprintw(_win, 6, 25, "H %i", (int)(100*sustain));
        mvwprintw(_win, 6, 34, "R %i", (int)(100*release));
    }

}

// PAGE 3: LFO
void draw_page_three(WINDOW* _win, T3::opz_device* _opz) {
    mvwprintw(_win,1, 1, "LFO    DEPTH   RATE    DEST    SHAPE");
    wattron(_win, COLOR_PAIR(4));
    mvwprintw(_win,2, 1, "       %s %s         %s",   hBar(7, (size_t)_opz->getActivePageParameters().lfo_depth).c_str(),
                                            hBar(7, (size_t)_opz->getActivePageParameters().lfo_speed).c_str(),
                                            T3::lfoShapeShapeString( _opz->getActivePageParameters().lfo_shape ).c_str());
    wattroff(_win, COLOR_PAIR(4));
    mvwprintw(_win,3, 1, "       %3i     %3i     %3s    %5s", 
                                            (int)((int)_opz->getActivePageParameters().lfo_depth / 2.55f), 
                                            (int)((int)_opz->getActivePageParameters().lfo_speed / 2.55f),
                                            T3::lfoDestinationShortString( _opz->getActivePageParameters().lfo_value ).c_str(),
                                            T3::lfoShapeShortString( _opz->getActivePageParameters().lfo_shape ).c_str() );
}

void draw_page_four(WINDOW* _win, T3::opz_device* _opz) {
    // PAGE 4: FX / PAN & LEVEL
    mvwprintw(_win, 1, 1, " FX  1       2");
    wattron(_win, COLOR_PAIR(4));
    mvwprintw(_win, 2, 1, "     %s %s", hBar(7, (size_t)_opz->getActivePageParameters().fx1).c_str(),
                                            hBar(7, (size_t)_opz->getActivePageParameters().fx2).c_str());
    wattroff(_win, COLOR_PAIR(4));
    mvwprintw(_win, 3, 1, "     %7i %7i", 
                                            (int)((int)_opz->getActivePageParameters().fx1 / 2.55f),
                                            (int)((int)_opz->getActivePageParameters().fx2 / 2.55f) );

    mvwprintw(_win, 5, 1, " PAN L             R");
    mvwprintw(_win, 6, 1, "     ");
    for (size_t i = 0; i < 15; i++) {
        size_t p = _opz->getActivePageParameters().pan;
        p = (p/254.0)*15;
        if (i + 2 > p  && i < p ) {
            wattron(_win, COLOR_PAIR(2));
            wprintw(_win, "|");
            wattroff(_win, COLOR_PAIR(2));
        }
        else {
            wattron(_win, COLOR_PAIR(4));
            wprintw(_win,".");
            wattroff(_win, COLOR_PAIR(4));
        }
    }

    mvwprintw(_win, 9, 1, " LEVEL     %03i", (int)( (int)_opz->getActivePageParameters().level / 2.55f));
    wattron(_win, COLOR_PAIR(4));
    mvwprintw(_win, 10, 1, "     %s", hBar(15, (size_t)_opz->getActivePageParameters().level).c_str());
    wattroff(_win, COLOR_PAIR(4));
}

void draw_track_params(WINDOW* _win, T3::opz_device* _opz) {
    mvwprintw(_win, 1,  2, "NOTE");
    mvwprintw(_win, 3,  2, "    LENGTH");
    mvwprintw(_win, 4,  2, "%10s", T3::noteLengthString( _opz->getActiveTrackParameters().note_length ).c_str() );
    mvwprintw(_win, 6,  2, "     STYLE");
    mvwprintw(_win, 7,  2, "%10s", T3::noteStyleString( _opz->getActiveTrackId(), _opz->getActivePageParameters().note_style ).c_str() );
    mvwprintw(_win, 10, 2, "  QUANTIZE");
    mvwprintw(_win, 11, 2, "%9i%%", (int)((int)_opz->getActiveTrackParameters().quantize / 2.55f));
    mvwprintw(_win, 13, 2, "PORTAMENTO");
    mvwprintw(_win, 14, 2, "%9i%%", (int)((int)_opz->getActivePageParameters().portamento / 2.55f) );
}
