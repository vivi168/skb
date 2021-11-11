#include <sys/types.h>
#include "input.h"
#include <libapi.h>

#define PAD_SELECT      1
#define PAD_L3          2
#define PAD_R3          4
#define PAD_START       8
#define PAD_UP          16
#define PAD_RIGHT       32
#define PAD_DOWN        64
#define PAD_LEFT        128
#define PAD_L2          256
#define PAD_R2          512
#define PAD_L1          1024
#define PAD_R1          2048
#define PAD_TRIANGLE    4096
#define PAD_CIRCLE      8192
#define PAD_CROSS       16384
#define PAD_SQUARE      32768

typedef struct _PADTYPE
{
    unsigned char   stat;
    unsigned char   len:4;
    unsigned char   type:4;
    unsigned short  btn;
    unsigned char   rs_x,rs_y;
    unsigned char   ls_x,ls_y;
} PADTYPE;

PADTYPE *pad;
u_char padbuff[2][34];

void iptm_update()
{
    u_short keys_pressed, keys_held, keys_released;

    // Parse controller input
    pad = (PADTYPE*)padbuff[0];

    // Only parse inputs when a controller is connected
    if( pad->stat == 0 )
    {
        // Only parse when a digital pad,
        // dual-analog and dual-shock is connected
        if( ( pad->type == 0x4 ) ||
            ( pad->type == 0x5 ) ||
            ( pad->type == 0x7 ) )
        {
            input_manager.old_keystate = input_manager.new_keystate;
            input_manager.new_keystate = pad->btn ^ 0xffff;

            keys_pressed =  (input_manager.old_keystate ^ input_manager.new_keystate) & input_manager.new_keystate;
            keys_held = input_manager.old_keystate & input_manager.new_keystate;
            keys_released = !input_manager.old_keystate & input_manager.new_keystate;

            // printf("%x %x %x\n", keys_pressed, keys_held, keys_released);

            if (keys_pressed & PAD_UP) {
                input_manager.keys_pressed |= (1UL << KEY_UP);
            } else if (!(keys_pressed & PAD_UP)) {
                input_manager.keys_pressed &= ~(1UL << KEY_UP);
            }

            if (keys_pressed & PAD_RIGHT) {
                input_manager.keys_pressed |= (1UL << KEY_RIGHT);
            } else if (!(keys_pressed & PAD_RIGHT)) {
                input_manager.keys_pressed &= ~(1UL << KEY_RIGHT);
            }

            if (keys_pressed & PAD_DOWN) {
                input_manager.keys_pressed |= (1UL << KEY_DOWN);
            } else if (!(keys_pressed & PAD_DOWN)) {
                input_manager.keys_pressed &= ~(1UL << KEY_DOWN);
            }

            if (keys_pressed & PAD_LEFT) {
                input_manager.keys_pressed |= (1UL << KEY_LEFT);
            } else if (!(keys_pressed & PAD_LEFT)) {
                input_manager.keys_pressed &= ~(1UL << KEY_LEFT);
            }
        }
    }

    // input_manager.old_keystate = input_manager.new_keystate;
}

void iptm_init()
{
    printf("[INFO]: pad init %x\n", 1UL);
    InitPAD( padbuff[0], 34, padbuff[1], 34 );

    padbuff[0][0] = padbuff[0][1] = 0xff;
    padbuff[1][0] = padbuff[1][1] = 0xff;

    StartPAD();
    ChangeClearPAD(1);

    input_manager.quit = FALSE;
    input_manager.new_keystate = 0;
    input_manager.old_keystate = 0;

    input_manager.keys_held = 0;
    input_manager.keys_pressed = 0;
    input_manager.keys_released = 0;
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
