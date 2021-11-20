#include <stdlib.h>
#include <stdio.h>

#include "renderer.h"
#include "level.h"
#include "input.h"

#ifndef PCVER
#include <libcd.h>
#define HEAP_SIZE (1024 * 1024)

char heap[HEAP_SIZE];
#endif

Level level;
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
        if (lvl_move_player(&level, DIR_UP)) level.steps++;
    }
    else if (iptm_is_pressed(KEY_DOWN)) {
        if (lvl_move_player(&level, DIR_DOWN)) level.steps++;
    }
    else if (iptm_is_pressed(KEY_LEFT)) {
        if (lvl_move_player(&level, DIR_LEFT)) level.steps++;
    }
    else if (iptm_is_pressed(KEY_RIGHT)) {
        if (lvl_move_player(&level, DIR_RIGHT)) level.steps++;
    }

    if (iptm_is_pressed(KEY_RESTART)) {
        lvl_reset(&level);
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
        lvl_check_level_done(&level);

        rdr_render(&level);
        rdr_delay(frame_start);
    }
}

int main(int argc, char** argv)
{
#ifndef PCVER
    InitHeap3((void*)&heap, HEAP_SIZE);
    CdInit();
#endif

    rdr_init();
    iptm_init();

    printf("[INFO]: init done !\n");

    lvl_init(&level, 0);
    printf("[INFO]: level init done !\n");

    mainloop();

    rdr_cleanup();

    return 0;
}
