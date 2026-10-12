#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <sys/socket.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>

#include <sced_types.h>
#include <model/settings.h>
#include <model/camera.h>
#include <model/detector.h>
#include <model/layers.h>

#include "actions.h"
#include "input.h"
#include "ui/layers.h"
#include "view/pick.h"
#include "view/view_state.h"

using namespace std;

// Owned by glced.cc.
extern long int doubleClickTime;
extern int socket_fd;
extern bool client_connected;
extern CEDsettings setting;

static enum {
    NO_MOVE,
    TURN_XY,
    ORIGIN
} move_mode;
static GLfloat mouse_x=0.;
static GLfloat mouse_y=0.;

void mouseWheel(int, int dir, int, int ){
    if(dir > 0){
        selectFromMenu(VIEW_ZOOM_IN);
    }else{
        selectFromMenu(VIEW_ZOOM_OUT);
    }
}

void mouse(int btn,int state,int x,int y){
    struct timeval tv;

    if(state!=MOUSE_DOWN){
        move_mode=NO_MOVE;
        return;
    }
    mouse_x=x;
    mouse_y=y;
    camera_drag_begin();

    switch(btn){
    case MOUSE_LEFT:
        view_request_redraw();
        gettimeofday(&tv, 0);
        //FIX IT: get the system double click time
        if( (tv.tv_sec*1000000+tv.tv_usec-doubleClickTime) < 300000 && (tv.tv_sec*1000000+tv.tv_usec-doubleClickTime) > 5){ //1000000=1sec

            int id;
            if(!ced_picking(x,y,&id)){
               if(client_connected){
                    send( socket_fd , &id , sizeof(int) , 0 );
                }
            }


        }else{
            if(setting.fixed_view == 0){ //dont rotate the view when in side or front projection
                move_mode=TURN_XY;
            }
        }
        doubleClickTime=tv.tv_sec*1000000+tv.tv_usec;
        return;
        case MOUSE_RIGHT:
          view_request_redraw();
          return;
        case MOUSE_MIDDLE:
          move_mode=ORIGIN;
          return;
        default:
          break;
    }
}

#define SELECT_FROM_MENU(key, action)                                          \
  case key:                                                                    \
    selectFromMenu(action);                                                    \
    break


void keypressed(unsigned char key, int x, int y) {
  // TODO: socket list for communicating with client

  switch (key) {
    SELECT_FROM_MENU('r', VIEW_RESET);
    SELECT_FROM_MENU('R', CED_RESET);
    SELECT_FROM_MENU('f', VIEW_FRONT);
    SELECT_FROM_MENU('F', TOGGLE_Z_PROJECTION);
    SELECT_FROM_MENU('s', VIEW_SIDE);
    SELECT_FROM_MENU('S', TOGGLE_PHI_PROJECTION);
    SELECT_FROM_MENU('+', VIEW_ZOOM_IN);
    SELECT_FROM_MENU('-', VIEW_ZOOM_OUT);

  case 'c':
  case 'C':
    GLfloat wx, wy, wz;
    if (!ced_get_selected(x, y, &wx, &wy, &wz)) {
      camera_center_on(wx, wy, wz);
      view_request_redraw();
    }
    break;

    SELECT_FROM_MENU('`', LAYER_ALL);

    SELECT_FROM_MENU('~', DETECTOR_ALL);

  case 'z':
    detector_cut_z_up();
    view_request_redraw();
    break;

  case 'Z':
    detector_cut_z_down();
    view_request_redraw();
    break;

  case '<':
    detector_trans_down();
    view_request_redraw();
    break;

  case '>':
    detector_trans_up();
    view_request_redraw();
    break;

  case 'm':
    detector_cut_angle_down();
    view_request_redraw();
    break;

  case 'M':
    detector_cut_angle_up();
    view_request_redraw();
    break;

  case 'h':
    view_toggle_help();
    view_request_redraw();
    break;
  default: {
    int layer = layer_from_key(key);
    if (layer >= 0) {
      toggle_layer(layer);
      view_request_redraw();
    } else {
      std::cerr << "Unknown keyboard shortcut: " << key << std::endl;
    }
  }
  }
}

void SpecialKey( int key, int, int ){
   switch (key) {
   case KEY_RIGHT:
    camera_shift_z(50.);
    break;
   case KEY_LEFT:
    camera_shift_z(-50.);
    break;

   case KEY_UP:
    camera_shift_y(50.);
    break;
   case KEY_DOWN:
    camera_shift_y(-50.);
    break;

   default:
      return;
   }
   view_request_redraw();
}


void motion(int x,int y){
    ViewSize window = get_window_size();
    if((move_mode == NO_MOVE) || !window.w || !window.h)
      return;

    if(move_mode == TURN_XY){
      camera_rotate((x-mouse_x)*180./window.w, (y-mouse_y)*180./window.h);

      //todo
    } else if (move_mode == ORIGIN){
        camera_pan(x-mouse_x, y-mouse_y, window.w, window.h);
    }
    view_request_redraw();
}
