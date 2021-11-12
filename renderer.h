#ifndef RENDERER_H
#define RENDERER_H

#include "level.h"

void rdr_init();
void rdr_create_texture();
void rdr_render(Level*);
void rdr_render_level(Level*);
void rdr_render_tile(Level*, int, int);
void rdr_cleanup();

unsigned int rdr_getticks();
void rdr_delay();

#endif
