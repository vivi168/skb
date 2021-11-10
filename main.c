#include <SDL2/SDL.h>
#include <stdio.h>

#include "renderer.h"
#include "level.h"
#include "input.h"

InputManager input_manager;
Level lvl;
int steps;
int quit;

#define FPS 144
#define TICKS_PER_FRAME (1000 / FPS)

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
    unsigned int frame_start, frame_time;

    quit = FALSE;
    steps = 0;

    while (!quit) {
        frame_start = SDL_GetTicks();

        iptm_poll_events();
        process_input();
        rdr_render(&lvl);

        if (lvl_done(&lvl))
            quit = TRUE;

        frame_time = SDL_GetTicks() - frame_start;
        if (TICKS_PER_FRAME > frame_time) {
            SDL_Delay(TICKS_PER_FRAME - frame_time);
        }
    }
}

int main(int argc, char** argv)
{
    char* filename;
    printf("%d", argc);

    if (argc == 2) {
        printf("%s", argv[1]);
        filename = argv[1];
    } else {
        filename = "level1.txt";
    }

    lvl_init(&lvl, filename);

    rdr_init();

    mainloop();

    rdr_cleanup();

    return 0;
}
