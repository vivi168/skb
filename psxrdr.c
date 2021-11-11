#include <stdlib.h>
#include <stdio.h>

#ifndef PCVER
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include "io.h"
#endif

#include "renderer.h"

#define SCREEN_W 320
#define SCREEN_H 240

#define TILE_SIZE 16
#define OTLEN 8

DISPENV disp[2];
DRAWENV draw[2];
int db = 0;

u_long ot[2][OTLEN];
char pribuff[2][32768];
char *nextpri;

typedef struct texture_t {
    u_long mode;
    u_char u, v;
    RECT prect, crect;

    u_short tpage, clut;
} Texture;

Texture texture;

void rdr_render_level(Level*);
void rdr_render_tile(int, int);
void rdr_render_sprite(RECT*, int, int);
void rdr_load_texture();

void rdr_init()
{
    printf("[INFO]: init\n");

    ResetGraph(0);

    // First buffer
    SetDefDispEnv(&disp[0], 0, 0, SCREEN_W, SCREEN_H);
    SetDefDrawEnv(&draw[0], 0, SCREEN_H, SCREEN_W, SCREEN_H);
    // Second buffer
    SetDefDispEnv(&disp[1], 0, SCREEN_H, SCREEN_W, SCREEN_H);
    SetDefDrawEnv(&draw[1], 0, 0, SCREEN_W, SCREEN_H);

    draw[0].isbg = 1;
    setRGB0(&draw[0], 63, 0, 127);

    draw[1].isbg = 1;
    setRGB0(&draw[1], 63, 0, 127);

    nextpri = pribuff[0];

    rdr_load_texture();

    // set initial tpage
    // draw[0].tpage = getTPage(texture.mode & 0x3, 0, texture.prect.x, texture.prect.y);
    // draw[1].tpage = getTPage(texture.mode & 0x3, 0, texture.prect.x, texture.prect.y);
    // draw[0].tpage = texture.tpage;
    // draw[1].tpage = texture.tpage;

    PutDrawEnv(&draw[!db]);

    FntLoad( 960, 0 );
    FntOpen( 0, 8, 320, 224, 0, 100 );
}

void rdr_load_texture()
{
    u_long file_size;
    int i;
    char *buff;

    TIM_IMAGE   *image;
    u_int       *filebuff;

    buff = load_file("\\SKB16.TIM;1", &file_size);
    if (buff == NULL) {
        printf("[ERROR]: error while loading texture\n");
        while(1);
    }

    OpenTIM((u_long*)buff);
    ReadTIM(image);

    // upload pixel data to framebuffer
    LoadImage(image->prect, image->paddr);
    DrawSync(0);

    // upload CLUT to framebuffer if any
    if (image->mode & 0x8) {
        LoadImage(image->crect, image->caddr);
        DrawSync(0);
    }

    // copy properties
    printf("[INFO]: %d %d %d\n", image->mode, image->prect->x, image->prect->y);
    texture.prect = *image->prect;
    texture.crect = *image->crect;
    texture.mode = image->mode;

    texture.u = (texture.prect.x % 0x40) << ( 2 - (texture.mode & 0x3));
    texture.v = (texture.prect.y & 0xff);

    texture.tpage = getTPage(texture.mode & 0x3, 0, texture.prect.x, texture.prect.y);
    texture.clut = getClut(texture.crect.x, texture.crect.y);

    printf("[INFO]: %d %d %d\n", texture.mode, texture.prect.x, texture.prect.y);

    free(buff);
}

void rdr_render(Level* level)
{
    // TODO

    ClearOTagR(ot[db], OTLEN);

    rdr_render_level(level);

    FntFlush(-1);
}

void rdr_render_level(Level* level)
{
    DR_TPAGE *tpage;
    int i, c;

    // player
    rdr_render_tile(3, level->player_pos);

    // crates
    for (i = 0; i < MAX_CRATES; i++) {
        c = level->crates_pos[i];

        if (level->tiles[c] == TARGET_T)
            rdr_render_tile(5, c);
        else
            rdr_render_tile(4, c);
    }

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

    tpage = (DR_TPAGE*)nextpri;
    SetDrawTPage(tpage, 0, 1, texture.tpage);
    addPrim(ot[db], tpage);
    nextpri += sizeof(DR_TPAGE);
}

void rdr_render_tile(int offset, int i)
{
    // TODO
    RECT src;
    int src_x;
    int dst_x, dst_y;

    src_x = (offset * TILE_SIZE);

    src.x = texture.u + src_x;
    src.y = texture.v;
    src.w = TILE_SIZE;
    src.h = TILE_SIZE;

    dst_x = TILE_SIZE * (i % LVL_W);
    dst_y = TILE_SIZE * (i / LVL_W);

    rdr_render_sprite(&src, dst_x, dst_y);
}

void rdr_render_sprite(RECT *src, int x, int y)
{
    SPRT *sprt;

    sprt = (SPRT*)nextpri;
    setSprt(sprt);

    setXY0(sprt, x, y);
    setWH(sprt, src->w, src->h);
    setUV0(sprt, src->x, src->y);
    setRGB0(sprt, 128, 128, 128);
    sprt->clut = texture.clut;

    addPrim(ot[db], sprt);
    nextpri += sizeof(SPRT);
}

void rdr_cleanup()
{
    // TODO
}

unsigned int rdr_getticks()
{
    // TODO
    return 0;
}

void rdr_delay(int frame_start)
{
    DrawSync(0);
    VSync(0);

    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);
    SetDispMask(1);
    DrawOTag(ot[db]+OTLEN-1);

    db = !db;
    nextpri = pribuff[db];
}
