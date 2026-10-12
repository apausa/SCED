#include <third_party/gl_font.h>

#include "view.h"
#include "view_state.h"

static SDL_Window *sdl_window = nullptr;
static ViewSize window_size = {500, 500};
static bool needs_redraw = false;
static bool show_help = false;
static const double background_color[4] = {1, 1, 1, 1}; // white
static const int font = FONT_M;

void view_set_window(SDL_Window *window){
    sdl_window = window;
}

SDL_Window *view_window(void){
    return sdl_window;
}

ViewSize get_window_size(void){
    return window_size;
}

void set_window_size(int w, int h){
    window_size.w = w;
    window_size.h = h;
}

void window_reshape(void){
    reshape(window_size.w, window_size.h);
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

const double *get_background_color(void){
    return background_color;
}

int get_font(void){
    return font;
}
