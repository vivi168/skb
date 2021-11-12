#include <stdlib.h>
#include <stdio.h>

#include "renderer.h"
#include "level.h"
#include "input.h"

#ifndef PCVER
#include <libcd.h>
#endif

Level lvl;
int quit;

void process_input()
{
    if (iptm_quit_requested() || iptm_is_pressed(KEY_QUIT)) {
        printf("[INFO]: quit requested\n");
#ifdef PCVER
        quit = TRUE;
#endif
    }

    if (iptm_is_pressed(KEY_UP)) {
        if (lvl_move_player(&lvl, DIR_UP)) lvl.steps++;
    }
    else if (iptm_is_pressed(KEY_DOWN)) {
        if (lvl_move_player(&lvl, DIR_DOWN)) lvl.steps++;
    }
    else if (iptm_is_pressed(KEY_LEFT)) {
        if (lvl_move_player(&lvl, DIR_LEFT)) lvl.steps++;
    }
    else if (iptm_is_pressed(KEY_RIGHT)) {
        if (lvl_move_player(&lvl, DIR_RIGHT)) lvl.steps++;
    }

    if (iptm_is_pressed(KEY_RESTART)) {
        lvl_reset(&lvl);
    }
}

void mainloop()
{
    unsigned int frame_start;

    quit = FALSE;

    while (!quit) {
        frame_start = rdr_getticks();

        iptm_poll_events();
        process_input();
        rdr_render(&lvl);

        if (lvl_done(&lvl))
#ifdef PCVER
            quit = TRUE; // TODO load next level
#else
            lvl_reset(&lvl);
#endif

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
        filename = "levels/level01.txt";
    }
#else
    filename = "\\LEVEL01.TXT;1";
    CdInit();
#endif
    rdr_init();
    iptm_init();

    printf("[INFO]: init done !\n");
    lvl_init(&lvl, filename);
    printf("[INFO]: lvl init done !\n");

    mainloop();

    rdr_cleanup();

    return 0;
}
