#include "view_state.h"

static SDL_Window *sdl_window = nullptr;
static bool needs_redraw = false;
static bool show_help = false;

void view_set_window(SDL_Window *window){
    sdl_window = window;
}

SDL_Window *view_window(void){
    return sdl_window;
}

void view_request_redraw(void){
    needs_redraw = true;
}

bool view_take_redraw_request(void){
    bool requested = needs_redraw;
    needs_redraw = false;
    return requested;
}

void view_toggle_help(void){
    show_help = !show_help;
}

bool view_help_shown(void){
    return show_help;
}
