/* SDL event pump for CED's event display server.
 * Split out of glced.cc: polls SDL input and the network socket,
 * drives redraws, and implements the borderless-window hit test. */

#include <SDL3/SDL.h>

#include <sced_types.h>

#include "view/view.h"
#include "view/view_state.h"
#include "controller/input.h"
#include "view/overlay.h"
#include "model/event.h"
#include "tcp_listener.h"

#include "event_loop.h"

void input_data(void *data){
    if(ced_process_input(data)>0){
        view_request_redraw();
    }
}

static const int kResizeBorder = 6;

SDL_HitTestResult SDLCALL ced_window_hit_test(SDL_Window *win, const SDL_Point *pt, void *data){
    int w, h;
    SDL_GetWindowSize(win, &w, &h);

    bool left = pt->x < kResizeBorder;
    bool right = pt->x >= w - kResizeBorder;
    bool top = pt->y < kResizeBorder;
    bool bottom = pt->y >= h - kResizeBorder;

    if(top && left) return SDL_HITTEST_RESIZE_TOPLEFT;
    if(top && right) return SDL_HITTEST_RESIZE_TOPRIGHT;
    if(bottom && left) return SDL_HITTEST_RESIZE_BOTTOMLEFT;
    if(bottom && right) return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
    if(left) return SDL_HITTEST_RESIZE_LEFT;
    if(right) return SDL_HITTEST_RESIZE_RIGHT;
    if(bottom) return SDL_HITTEST_RESIZE_BOTTOM;
    if(top) return SDL_HITTEST_RESIZE_TOP;

    if(pt->y < CED_TITLE_BAR_HEIGHT) return SDL_HITTEST_DRAGGABLE;

    return SDL_HITTEST_NORMAL;
}

void mainLoop(void) {
    bool running = true;
    view_request_redraw();

    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {

            case SDL_EVENT_QUIT:
                running = false;
                break;

            case SDL_EVENT_WINDOW_RESIZED: // Replaces glutReshapeFunc(reshape)
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                reshape(ev.window.data1, ev.window.data2);
                view_request_redraw();
                break;

            case SDL_EVENT_TEXT_INPUT: // Printable characters (replaces glutKeyboardFunc(keypressed))
                keypressed((unsigned char)ev.text.text[0], 0, 0);
                break;

            case SDL_EVENT_KEY_DOWN: { // Special keys and Ctrl + letter shortcuts (replaces glutSpecialFunc(SpecialKey))
                SDL_Keycode sym = ev.key.key;
                int special = -1;
                switch (sym) {
                    case SDLK_RIGHT: special = KEY_RIGHT; break;
                    case SDLK_LEFT: special = KEY_LEFT; break;
                    case SDLK_UP: special = KEY_UP; break;
                    case SDLK_DOWN: special = KEY_DOWN; break;
                    case SDLK_PAGEUP: special = KEY_PAGE_UP; break;
                    case SDLK_PAGEDOWN: special = KEY_PAGE_DOWN; break;
                    case SDLK_HOME: special = KEY_HOME; break;
                    case SDLK_END: special = KEY_END; break;
                    case SDLK_INSERT: special = KEY_INSERT; break;
                    default: break;
                }
                if (sym == SDLK_ESCAPE) {
                    running = false;
                } else if (special >= 0) {
                    SpecialKey(special, 0, 0);
                } else if (ev.key.mod & SDL_KMOD_CTRL) {
                    SDL_Scancode sc = ev.key.scancode;
                    if (sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_Z) {
                        unsigned char key = (unsigned char)(sc - SDL_SCANCODE_A + 1);
                        keypressed(key, 0, 0);
                    }
                }
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                int btn   = ev.button.button - 1;
                int state = (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) ? MOUSE_DOWN : MOUSE_UP;
                mouse(btn, state, (int)ev.button.x, (int)ev.button.y);
                break;
            }

            case SDL_EVENT_MOUSE_MOTION:
                if (ev.motion.state != 0) {
                    motion((int)ev.motion.x, (int)ev.motion.y);
                }
                break;

            case SDL_EVENT_MOUSE_WHEEL: {
                int dir = (ev.wheel.y > 0) ? 1 : -1;
                mouseWheel(0, dir, 0, 0);
                break;
            }

            default:
                break;
            }
        }

        net_poll();

        if (view_take_redraw_request()) {
            display();
        } else {
            SDL_Delay(1);
        }
    }
}
