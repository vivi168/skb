#include <stdlib.h>
#include <stdio.h>

#ifndef PCVER
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#endif

#include "renderer.h"

#define TILE_SIZE 48

void rdr_render_level(Level*);
void rdr_render_tile(int, int);

void rdr_init()
{
    printf("init\n");

    ResetGraph(0);

    // TODO
}


void rdr_render(Level* level)
{
    // TODO

    rdr_render_level(level);
}

void rdr_render_level(Level* level)
{
    int i, c;

    for (i = 0; i < LVL_SIZE; i++) {
        switch (level->tiles[i]) {
        case GROUND_T:
            rdr_render_tile(0, i);
            break;
        case TARGET_T:
            rdr_render_tile(1, i);
            break;
        case WALL_T:
            rdr_render_tile(2, i);
            break;
        }
    }

    // crates
    for (i = 0; i < MAX_CRATES; i++) {
        c = level->crates_pos[i];

        if (level->tiles[c] == TARGET_T)
            rdr_render_tile(5, c);
        else
            rdr_render_tile(4, c);
    }

    // player
    rdr_render_tile(3, level->player_pos);
}

void rdr_render_tile(int offset, int i)
{
    // TODO
}

void rdr_cleanup()
{
    // TODO
}

unsigned int rdr_getticks()
{
    return 0;
}

void rdr_delay(int frame_start)
{
    // TODO
}
