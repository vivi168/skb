#include <stdlib.h>
#include <stdio.h>

#include "level.h"

void fill_ground(Level*);
void empty_crate_pos(Level*);
int lvl_move_crate(Level*, int, Direction);

void lvl_init(Level* level, const char* filename)
{
    FILE* fp;
    char c;

    char *pdef = &level->def[0];

    level->size = 0;

    fill_ground(level);
    empty_crate_pos(level);

    fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Error while reading level file %s", filename);
        exit(EXIT_FAILURE);
    }

    while ((c = getc(fp)) != EOF) {
        *pdef++ = c;

        level->size++;
        if (level->size > LVL_SIZE) {
            fprintf(stderr, "Level too large");
            exit(EXIT_FAILURE);
        }
    }

    lvl_reset(level);

    fclose(fp);
}

void lvl_reset(Level* level)
{
    char c;
    int *pcrate_pos = &level->crates_pos[0];
    int k, l;

    level->w = 0; level->h = 0;
    level->crate_count = 0;
    k = 0; l = 0;

    for (int i = 0; i < level->size; i++) {
        c = level->def[i];
        printf("%x ", c);
        switch(c) {
        case GROUND_CHAR:
            break;
        case TARGET_CHAR:
            level->tiles[k] = TARGET_T;
            break;
        case TARGET_PCHAR:
            level->tiles[k] = TARGET_T;
            level->player_pos = k;
            break;
        case TARGET_CCHAR:
            level->tiles[k] = TARGET_T;
            *pcrate_pos++ = k;
            level->crate_count++;
            break;
        case WALL_CHAR:
            level->tiles[k] = WALL_T;
            break;
        case CRATE_CHAR:
            *pcrate_pos++ = k;
            level->crate_count++;
            break;
        case PLAYER_CHAR:
            level->player_pos = k;
            break;
        case NEWLINE_CHAR:
            if (l > level->w) level->w = l;
            level->h++;

            k += LVL_W - l;
            l = 0;
            continue;
        }

        l++;
        k++;
    }
}

void fill_ground(Level* level)
{
    for (int i = 0; i < LVL_SIZE; i++) {
        level->tiles[i] = GROUND_T;
    }
}

void empty_crate_pos(Level* level)
{
    for (int i = 0; i < MAX_CRATES; i++) {
        level->crates_pos[i] = -1;
    }
}

int lvl_move_player(Level* level, Direction dir)
{
    int prev_pos = level->player_pos;

    switch (dir) {
    case DIR_UP:
        level->player_pos -= LVL_W;
        break;
    case DIR_LEFT:
        level->player_pos --;
        break;
    case DIR_DOWN:
        level->player_pos += LVL_W;
        break;
    case DIR_RIGHT:
        level->player_pos++;
        break;
    }

    for (int i = 0; i < level->crate_count; i++) {
        if (level->player_pos == level->crates_pos[i]) {
            int moved = lvl_move_crate(level, i, dir);
            if (!moved) level->player_pos = prev_pos;
            break;
        }
    }

    if (level->player_pos < 0 || level->player_pos > LVL_SIZE) {
        level->player_pos = prev_pos;
    }

    if (level->tiles[level->player_pos] == WALL_T) level->player_pos = prev_pos;

    return prev_pos != level->player_pos;
}

int lvl_move_crate(Level* level, int i, Direction dir)
{
    int crate_pos = level->crates_pos[i];

    switch (dir) {
    case DIR_UP:
        crate_pos -= LVL_W;
        break;
    case DIR_LEFT:
        crate_pos--;
        break;
    case DIR_DOWN:
        crate_pos += LVL_W;
        break;
    case DIR_RIGHT:
        crate_pos++;
        break;
    }

    if (crate_pos < 0 || crate_pos > LVL_SIZE) {
        crate_pos = level->crates_pos[i];
    }

    if (level->tiles[crate_pos] == WALL_T) crate_pos = level->crates_pos[i];

    for (int c = 0; c < level->crate_count; c++) {
        if (c != i && crate_pos == level->crates_pos[c]) {
            crate_pos = level->crates_pos[i];
            break;
        }
    }

    int moved = crate_pos != level->crates_pos[i];

    level->crates_pos[i] = crate_pos;

    return moved;
}

int lvl_done(Level* level)
{
    int cleared = 0;

    for (int i = 0; i < level->crate_count; i++) {
        int c = level->crates_pos[i];
        if (level->tiles[c] == TARGET_T)
            cleared++;
    }

    return cleared == level->crate_count;
}
