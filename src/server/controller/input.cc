#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

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
#include "keymap.h"
#include "tcp_listener.h"
#include "view/pick.h"
#include "view/view_state.h"

using namespace std;

extern CEDsettings setting;

static enum {
    NO_MOVE,
    TURN_XY,
    ORIGIN
} move_mode;
static GLfloat mouse_x=0.;
static GLfloat mouse_y=0.;

// Two left presses closer together than this are a double click.
static const long int DOUBLE_CLICK_TIME_US = 300000;
static long int doubleClickTime=0; // when the last left press happened, in microseconds

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
        if( (tv.tv_sec*1000000+tv.tv_usec-doubleClickTime) < DOUBLE_CLICK_TIME_US && (tv.tv_sec*1000000+tv.tv_usec-doubleClickTime) > 5){ //1000000=1sec

            int id;
            if(!ced_picking(x,y,&id)){
                net_send_selected_id(id);
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

void keypressed(unsigned char key, int x, int y) {
  // TODO: socket list for communicating with client

  int action = keymap_action(key);
  switch (action) {
  case VIEW_RESET:
  case CED_RESET:
  case VIEW_FRONT:
  case VIEW_SIDE:
  case TOGGLE_Z_PROJECTION:
  case TOGGLE_PHI_PROJECTION:
  case VIEW_ZOOM_IN:
  case VIEW_ZOOM_OUT:
  case LAYER_ALL:
  case DETECTOR_ALL:
    selectFromMenu(action);
    break;

  case VIEW_CENTER:
    GLfloat wx, wy, wz;
    if (!ced_get_selected(x, y, &wx, &wy, &wz)) {
      camera_center_on(wx, wy, wz);
      view_request_redraw();
    }
    break;

  case DETECTOR_CUT_Z_UP:
    detector_cut_z_up();
    view_request_redraw();
    break;

  case DETECTOR_CUT_Z_DOWN:
    detector_cut_z_down();
    view_request_redraw();
    break;

  case DETECTOR_TRANS_DOWN:
    detector_trans_down();
    view_request_redraw();
    break;

  case DETECTOR_TRANS_UP:
    detector_trans_up();
    view_request_redraw();
    break;

  case DETECTOR_CUT_ANGLE_DOWN:
    detector_cut_angle_down();
    view_request_redraw();
    break;

  case DETECTOR_CUT_ANGLE_UP:
    detector_cut_angle_up();
    view_request_redraw();
    break;

  case HELP_TOGGLE:
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
