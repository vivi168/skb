#ifndef INPUT_H
#define INPUT_H

#define TRUE 1
#define FALSE 0

#define KEY_UP      0
#define KEY_DOWN    1
#define KEY_LEFT    2
#define KEY_RIGHT   3
#define KEY_RESTART 4
#define KEY_QUIT    5

typedef struct input_manager_t {
    int quit;
    unsigned int new_keystate, old_keystate;
    unsigned int keys_pressed, keys_held, keys_released;
} InputManager;

extern InputManager input_manager;

void iptm_init();
void iptm_update();
void iptm_poll_events();
int iptm_quit_requested();
int iptm_is_held(int);
int iptm_is_pressed(int);
int iptm_is_released(int);

#endif
