#ifndef RENDERER_H
#define RENDERER_H

#include "level.h"

void rdr_init();
void rdr_create_window();
void rdr_create_tileset();

void rdr_render(Level*);
void rdr_render_level(Level*);
void rdr_render_tile(int, int);

void rdr_cleanup();

#endif
