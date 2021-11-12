#ifndef RENDERER_H
#define RENDERER_H

#include "level.h"

// tiles indices in spritesheet
#define GROUND_IDX 0
#define TARGET_IDX 1
#define WALL_IDX 2
#define PLAYER_IDX 3
#define CRATEG_IDX 4
#define CRATET_IDX 5

void rdr_init();
void rdr_create_texture();
void rdr_render(Level*);
void rdr_render_level(Level*);
void rdr_render_tile(int, int, int, int);
void rdr_cleanup();

unsigned int rdr_getticks();
void rdr_delay();

#endif
