
#include <iostream>
#include <fstream>

#include <thread>
#include <atomic>
#include <mutex>
#include <signal.h>

#include <ncurses.h>

#include "libopz/opz_rtmidi.h"

#include "osc.h"
#include "tools.h"

#include "pages.h"
#include "mixer.h"
#include "project.h"
#include "tempo.h"
#include "mic.h"

std::vector<WINDOW*> windows;

void handle_winch(int sig) {
    endwin();
    refresh();
    for (size_t i = 0; i < windows.size(); i++)
        wrefresh(windows[i]);
    wresize( windows[5], LINES-2, COLS-80 );
}

int main(int argc, char** argv) {
    std::atomic<bool> keepRunnig(true);

    T3::opz_rtmidi opz;
    opz.connect();
    
    initscr();
    start_color();
    use_default_colors();

    init_color(COLOR_MAGENTA, 1000, 100, 100);
    init_color(COLOR_YELLOW, 800, 800, 800);
    init_color(COLOR_GREEN, 600, 600, 600);
    init_color(COLOR_BLUE, 300, 300, 300);

    init_pair(1, COLOR_MAGENTA, -1);
    init_pair(2, COLOR_RED, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_GREEN, -1);
    init_pair(5, COLOR_BLUE, -1);

    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    bool large_screen = false;
    large_screen = (COLS >= 159); 

    windows.push_back( newwin(5, 41, 1, 0) );    //  PAGE ONE
    windows.push_back( newwin(8, 41, 6, 0) );    //  PAGE TWO
    windows.push_back( newwin(5, 66, 14, 0) );   //  PAGE THREE
    windows.push_back( newwin(13, 25, 1, 41) );  //  PAGE FOUR
    
    windows.push_back( newwin(18, 14, 1, 66) );  //  STEP / NOTE
    
    // Extra window that can be display on the side or on top of the pages depending of the size of the terminal
    windows.push_back( newwin((large_screen ? LINES-2 : 0), (large_screen ? COLS-80 : 0) , 1, (large_screen ? 80 : 0) ) );

    signal(SIGWINCH, handle_winch);

    bool change = true;
    bool change_data = true;
    bool pressing_track = false;
    bool pressing_project = false;
    bool pressing_mixer = false;
    bool pressing_tempo = false;
    bool mic_on = false;
    
    // Listen to key events (no cc, neighter notes)
    opz.setEventCallback( [&](T3::opz_event_id _id, int _value) {
        change = true;

        if (_id == T3::KEY_TRACK)           pressing_track = _value;
        else if (_id == T3::KEY_PROJECT)    pressing_project = _value;
        else if (_id == T3::KEY_MIXER)      pressing_mixer = _value;
        else if (_id == T3::KEY_TEMPO)      pressing_tempo = _value;
        else if (_id == T3::MICROPHONE_MODE_CHANGE) mic_on = _value != 0;
        else if (_id == T3::PATTERN_DOWNLOADED || _id == T3::PATTERN_CHANGE || _id == T3::TRACK_CHANGE || _id == T3::SEQUENCE_CHANGE || _id == T3::PAGE_CHANGE || _id == T3::PARAMETER_CHANGE ) change_data = true;
    
        // send(_id, _value);
    } );

    opz.setMidiCallback( [](T3::midi_id _id, size_t _channel, size_t _key, size_t _value) {
        send(_id, _channel, _key, _value);
    } );

    std::thread waitForKeys([&](){
        char ch;
        while ( true ) {
            ch = getch();
            
            if (ch == 'x') {
                keepRunnig = false;
                keepRunnig.store(false);
                break;
            }
        }
    });

    refresh();

    while (keepRunnig.load()) {
        opz.update();

        if (!change)
            continue;

        T3::opz_pattern pattern = opz.getActivePattern();
        T3::opz_track_id track_id = opz.getActiveTrackId();

        std::string title_name = T3::toString(track_id);

        if (mic_on) title_name = "MICROPHONE";
        else if (pressing_project) title_name = "PROJECTS";
        else if (pressing_mixer)   title_name = "MIXER";
        else if (pressing_tempo)   title_name = "TEMPO";

        erase();
        mvprintw(0, COLS/2 - title_name.size()/2, "%s", title_name.c_str() );

        size_t step_count = opz.getActiveTrackParameters().step_count;
        size_t step_length = opz.getActiveTrackParameters().step_length;

        if (opz.isPlaying()) {
            size_t step = (opz.getActiveStepId() / step_length) % step_count;
            mvprintw(LINES-4, 2 + step * 4 + ( (step/4) * 4 ) , "[ ]");
        }
        
        for (size_t i = 0; i < step_count; i++) {
            size_t x = 3 + i * 4 + ( (i/4) * 4 );
            mvprintw(LINES-5, x, "%02i", i + 1 );
            size_t note = opz.getNoteIdOffset(track_id, i);
            if ( pattern.note[ note ].note == 0xFF)
                mvprintw(LINES-4, x, "-");
            else {
                attron(COLOR_PAIR(2));
                mvprintw(LINES-4, x, "o");
                attroff(COLOR_PAIR(2));
            }
        }

        mvprintw(LINES-2, 0, "STEP COUNT %2i      STEP LENGTH %2i                                        SUM %2i", 
                                step_count, step_length, step_count * step_length);
        mvprintw(LINES-1, COLS/2 - 3, "%s %02i", ((opz.isPlaying())? "|> " : "[ ]"), opz.getActiveStepId() + 1 );
        refresh();

        if (pressing_project)       draw_project(windows[5], &opz);
        else if (pressing_mixer)    draw_mixer(windows[5], &opz);
        else if (pressing_tempo)    draw_tempo(windows[5], &opz);
        else if (mic_on)            draw_mic(windows[5], &opz);
        else if (large_screen)      draw_project(windows[5], &opz);

        if ( large_screen || (!mic_on && !pressing_project && !pressing_mixer && !pressing_tempo)){
            // werase(windows[5]);

            if (pressing_track)
                wattron(windows[4], COLOR_PAIR(2));

            size_t page = (size_t)opz.getActivePageId();

            for (size_t i = 0; i < 5; i++) {
                if (!pressing_track && i == page)
                    wattron(windows[i], COLOR_PAIR(1));
                box(windows[i], 0, 0);
                wattroff(windows[i], COLOR_PAIR(1));
            }

            if (page == 0 || change_data) draw_page_one(windows[0], &opz);
            if (page == 1 || change_data) draw_page_two(windows[1], &opz);
            if (page == 2 || change_data) draw_page_three(windows[2], &opz);
            if (page == 3 || change_data) draw_page_four(windows[3], &opz);

            if (pressing_track || change_data) draw_track_params(windows[4], &opz);

            for (size_t i = 0; i < 5; i++)
                wrefresh(windows[i]);

            change = false;
            change_data = false;
        }
    }
    
    waitForKeys.join();
    endwin();
    opz.disconnect();

    exit(0);
}
