/* "C" event display.
 * Communications related part. */
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <sced_types.h>
#include <settings.h>
#include <SDL3/SDL.h>
#include <third_party/gl_font.h>
#include "controller/input.h"

#include <iostream>

#include "controller/actions.h"
#include "view/view.h"
#include "cli.h"
#include "controller/event_loop.h"
#include "tcp_listener.h"
#include "view/draw/draw.h"

using namespace std;

//*************** global variables ***************************************//
extern CEDsettings setting;

int showHelp=0;
long int doubleClickTime=0;
extern int SELECTED_ID ;

void defaultSettings(void){
    setting.persp=true;
    setting.antia=false;

    setting.win_w=500;
    setting.win_h=500;
    setting.show_axes=true;

    for(int i=0;i<4;i++){
        setting.bgcolor[i]=1; //white
    }

    setting.font=FONT_M;

    for(int i=0;i<CED_MAX_LAYER;i++){
        setting.layer[i]=true; // turn all layers on
    }

    setting.detector_trans=0.8;
    setting.detector_cut_angle=0;
    setting.detector_cut_z=7000;

    setting.phi_projection=false;
    setting.z_projection=false;
    setting.fixed_view=false;

    mm=mm_reset;

    setting.zoom=mm.sf;

    std::cout << "Set options to default settings" << std::endl;
}

extern int socket_fd;
extern void (*socket_fn)(void);
extern bool client_connected;

bool ced_needs_redraw = false;
SDL_Window* ced_sdl_window = nullptr;

GLfloat window_width = 0.;
GLfloat window_height = 0.;

// ********** function definitions  (rest of file) ************************** //


int main(int argc,char *argv[]){
    #ifndef SDL_PLATFORM_APPLE
        setenv("SDL_VIDEODRIVER", "wayland", 0);

        // SDL's Wayland backend initializes xkbcommon directly as part of SDL_Init() to handle
        // keyboard input. The key4hep stack sets XKB_CONFIG_ROOT with a :, which xkbcommon
        // interprets as an empty search path entry and fails to create an XKB context, cascading
        // into SDL Init returning -1. The following code removes this character.
        const char *xkb = getenv("XKB_CONFIG_ROOT");

        if (xkb) {
            std::string s(xkb);

            if (!s.empty() && s.back() == ':') {
                s.pop_back();
                setenv("XKB_CONFIG_ROOT", s.c_str(), 1);
            }
        }
    #endif

    mm_reset=mm;

    SDL_Init(SDL_INIT_VIDEO);
    // SDL's Wayland backend uses EGL which defaults to OpenGL ES. The following code creates a desktop
    // OpenGL compatibility profile context.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

    defaultSettings();

    parseCliArgs(argc, argv);

    ced_register_elements();

    char *p;
    p = getenv ( "CED_PORT" );
    if(p != NULL){
        printf("Try to use user defined port %s.\n", p);
        tcp_server(atoi(p),input_data);
    }else{
        tcp_server(7286,input_data);
    }


    ced_sdl_window = SDL_CreateWindow(
        "C Event Display (CED)",
        setting.win_w,
        setting.win_h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS
    );

    SDL_SetWindowHitTest(ced_sdl_window, ced_window_hit_test, nullptr); // Implement drag and resize via hit-testing since the window is now borderless

    SDL_GLContext gl_context = SDL_GL_CreateContext(ced_sdl_window); // SDL separates window creation from context creation
    
    if (!gl_context) {
        fprintf(
            stderr,
            "SDL_GL_CreateContext failed: %s\n",
            SDL_GetError()
        );
        SDL_DestroyWindow(ced_sdl_window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(1); // vsync control
    SDL_StartTextInput(ced_sdl_window);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    init();
    font_init();

    reshape(setting.win_w, setting.win_h);
    mainLoop(gl_context);

    return 0;
}
