#include <stdio.h>

#include "renderer.h"
#include "level.h"
#include "input.h"

InputManager input_manager;
Level lvl;
int steps;
int quit;

void process_input()
{
    if (iptm_quit_requested() || iptm_is_pressed(KEY_QUIT)) {
        quit = TRUE;
    }

    if (iptm_is_pressed(KEY_UP)) {
        if (lvl_move_player(&lvl, DIR_UP)) steps++;
    }
    if (iptm_is_pressed(KEY_DOWN)) {
        if (lvl_move_player(&lvl, DIR_DOWN)) steps++;
    }
    if (iptm_is_pressed(KEY_LEFT)) {
        if (lvl_move_player(&lvl, DIR_LEFT)) steps++;
    }
    if (iptm_is_pressed(KEY_RIGHT)) {
        if (lvl_move_player(&lvl, DIR_RIGHT)) steps++;
    }

    if (iptm_is_pressed(KEY_RESTART)) {
        lvl_reset(&lvl);
    }
}

void mainloop()
{
    unsigned int frame_start;

    quit = FALSE;
    steps = 0;

    while (!quit) {
        frame_start = rdr_getticks();

        iptm_poll_events();
        process_input();
        rdr_render(&lvl);

        if (lvl_done(&lvl))
            quit = TRUE;

        rdr_delay(frame_start);
    }
}

int main(int argc, char** argv)
{
    char* filename;
#ifdef PCVER
    if (argc == 2) {
        filename = argv[1];
    } else {
        filename = "levels/level1.txt";
    }
#else
    filename = "level1.txt";
#endif
    lvl_init(&lvl, filename);

    rdr_init();

    mainloop();

    rdr_cleanup();

    return 0;
}
