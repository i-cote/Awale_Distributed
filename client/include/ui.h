#ifndef UI_H
#define UI_H

#include "app.h"

#define DEFAULT_COLOR 1
#define PLAYING_COLOR 2
#define SELECTION_COLOR 3

struct ui_window {
    void (*open)(struct app_state*);
    void (*on_key_press)(struct app_state*, int);
    void (*update)(struct app_state*, enum event*);
    void (*close)(struct app_state*);
};



void ui_init(void);
void ui_close(void);
#endif
