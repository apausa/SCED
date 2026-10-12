#pragma once

struct SDL_Window;

// The window the view draws into. main() sets it once the window exists.
void view_set_window(SDL_Window *window);
SDL_Window *view_window(void);

struct ViewSize {
    int w; // pixel
    int h;
};

// The window size. It is 500x500 until reshape() is given another size.
ViewSize get_window_size(void);

// Called by reshape().
void set_window_size(int w, int h);

// Runs reshape() again with the current window size.
void window_reshape(void);

// Ask for a frame. The loop draws it on its next pass.
void view_request_redraw(void);

// True if a frame was asked for since the last call.
bool view_take_redraw_request(void);

void view_toggle_help(void);
bool view_help_shown(void);

// The background colour (what glClear() paints), as r, g, b, a.
const double *get_background_color(void);
