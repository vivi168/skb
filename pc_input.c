#include <SDL2/SDL.h>

#include "input.h"

InputManager input_manager;

void iptm_update()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_QUIT:
            input_manager.quit = TRUE;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
                case SDLK_UP:
                    input_manager.new_keystate |= (1UL << KEY_UP);
                    break;
                case SDLK_RIGHT:
                    input_manager.new_keystate |= (1UL << KEY_RIGHT);
                    break;
                case SDLK_DOWN:
                    input_manager.new_keystate |= (1UL << KEY_DOWN);
                    break;
                case SDLK_LEFT:
                    input_manager.new_keystate |= (1UL << KEY_LEFT);
                    break;
                case SDLK_r:
                    input_manager.new_keystate |= (1UL << KEY_RESTART);
                    break;
                case SDLK_ESCAPE:
                    input_manager.new_keystate |= (1UL << KEY_QUIT);
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym) {
                case SDLK_UP:
                    input_manager.new_keystate &= ~(1UL << KEY_UP);
                    break;
                case SDLK_RIGHT:
                    input_manager.new_keystate &= ~(1UL << KEY_RIGHT);
                    break;
                case SDLK_DOWN:
                    input_manager.new_keystate &= ~(1UL << KEY_DOWN);
                    break;
                case SDLK_LEFT:
                    input_manager.new_keystate &= ~(1UL << KEY_LEFT);
                    break;
                case SDLK_r:
                    input_manager.new_keystate &= ~(1UL << KEY_RESTART);
                    break;
                case SDLK_ESCAPE:
                    input_manager.new_keystate &= ~(1UL << KEY_QUIT);
                    break;
            }
            break;
        default:
            break;
        }
    }

    input_manager.keys_released = (input_manager.old_keystate ^ input_manager.new_keystate) ^ input_manager.new_keystate;
    input_manager.keys_pressed =  (input_manager.old_keystate ^ input_manager.new_keystate) & input_manager.new_keystate;
    input_manager.keys_held = input_manager.old_keystate & input_manager.new_keystate;

    input_manager.old_keystate = input_manager.new_keystate;
}

void iptm_init()
{
    input_manager.quit = FALSE;
    input_manager.new_keystate = 0;
    input_manager.old_keystate = 0;
}

void iptm_poll_events()
{
    iptm_update();
}

int iptm_quit_requested() {
    return input_manager.quit;
}

int iptm_is_held(int k) {
    return (1 << k) & input_manager.keys_held;
}

int iptm_is_pressed(int k) {
    return (1 << k) & input_manager.keys_pressed;
}

int iptm_is_released(int k) {
    return (1 << k) & input_manager.keys_released;
}
