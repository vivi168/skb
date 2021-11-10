#ifndef RENDERER_H
#define RENDERER_H

#include "level.h"

void rdr_init();
void rdr_render(Level*);
void rdr_cleanup();

unsigned int rdr_getticks();
void rdr_delay();

#endif
