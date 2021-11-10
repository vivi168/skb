#include "input.h"

void iptm_update()
{
    // TODO
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
