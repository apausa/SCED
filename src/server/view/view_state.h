#pragma once

struct SDL_Window;

// The window the view draws into. main() sets it once the window exists.
void view_set_window(SDL_Window *window);
SDL_Window *view_window(void);

// Ask for a frame. The loop draws it on its next pass.
void view_request_redraw(void);

// True if a frame was asked for since the last call.
bool view_take_redraw_request(void);

void view_toggle_help(void);
bool view_help_shown(void);
