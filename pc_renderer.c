#include <SDL2/SDL.h>
#include <stdio.h>

#include "renderer.h"

#define TILE_SIZE 48
#define FPS 144
#define TICKS_PER_FRAME (1000 / FPS)

SDL_Window* sdl_window;
SDL_Renderer* sdl_renderer;
SDL_Texture* tileset;

void rdr_create_window();

void rdr_init()
{
    printf("init\n");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Unable to init SDL\n");
        exit(EXIT_FAILURE);
    }

    rdr_create_window();
    rdr_create_texture();
}

void rdr_create_window()
{
    printf("create window\n");

    const char* title = "Sokoban";
    const int width = LVL_W * TILE_SIZE;
    const int height = LVL_H * TILE_SIZE;

    sdl_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

    if (sdl_window == NULL) {
        fprintf(stderr, "Error while create SDL_Window\n");
        exit(EXIT_FAILURE);
    }

    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);

    if (sdl_renderer == NULL) {
        fprintf(stderr, "Error while creating SDL_Renderer\n");
        exit(EXIT_FAILURE);
    }
}

void rdr_create_texture()
{
    printf("create tileset\n");

    SDL_Surface* image = SDL_LoadBMP("sokoban48.bmp");
    tileset = SDL_CreateTextureFromSurface(sdl_renderer, image);

    if (tileset == NULL) {
        fprintf(stderr, "Error while creating Texture\n");
        exit(EXIT_FAILURE);
    }

    SDL_FreeSurface(image);
}

void rdr_render(Level* level)
{
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl_renderer);

    rdr_render_level(level);

    SDL_RenderPresent(sdl_renderer);
}

void rdr_render_level(Level* level)
{
    int hoff = level->hoff * TILE_SIZE;
    int voff = level->voff * TILE_SIZE;

    for (int i = 0; i < LVL_SIZE; i++) {
        switch (level->tiles[i]) {
        case VOID_T: break;
        case GROUND_T:
            rdr_render_tile(hoff, voff, 0, i);
            break;
        case TARGET_T:
            rdr_render_tile(hoff, voff, 1, i);
            break;
        case WALL_T:
            rdr_render_tile(hoff, voff, 2, i);
            break;
        }
    }

    // crates
    for (int i = 0; i < level->crate_count; i++) {
        int c = level->crates_pos[i];

        if (level->tiles[c] == TARGET_T)
            rdr_render_tile(hoff, voff, 5, c);
        else
            rdr_render_tile(hoff, voff, 4, c);
    }

    // player
    rdr_render_tile(hoff, voff, 3, level->player_pos);
}

void rdr_render_tile(int hoff, int voff, int offset, int i)
{
    SDL_Rect src = { offset * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
    SDL_Rect dst = { TILE_SIZE * (i % LVL_W) + hoff,
                     TILE_SIZE * (i / LVL_W) + voff,
                     TILE_SIZE, TILE_SIZE };

    SDL_RenderCopy(sdl_renderer, tileset, &src, &dst);
}

void rdr_cleanup()
{
    printf("cleanup\n");

    SDL_DestroyTexture(tileset);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);

    SDL_Quit();
}

unsigned int rdr_getticks()
{
    return SDL_GetTicks();
}

void rdr_delay(int frame_start)
{
    unsigned int frame_time;

    frame_time = rdr_getticks() - frame_start;
    if (TICKS_PER_FRAME > frame_time) {
        SDL_Delay(TICKS_PER_FRAME - frame_time);
    }
}
