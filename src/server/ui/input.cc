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
#include <settings.h>

#include "actions.h"
#include "input.h"
#include "layers.h"
#include "selection.h"

using namespace std;

// Owned by glced.cc.
extern long int doubleClickTime;
extern bool ced_needs_redraw;
extern int showHelp;
extern int socket_fd;
extern bool client_connected;
extern GLfloat window_width;
extern GLfloat window_height;
extern CEDsettings setting;

CameraState mm = {
    30.,
    150.,
    0.1, // decrease zoom, set redraw scale a lot smaller
    { 0., 0., 0. },
    0.,
    0.,
    { 0., 0., 0. },
};
CameraState mm_reset;

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
    mm.ha_start=mm.ha;
    mm.va_start=mm.va;
    mm.mv_start=mm.mv;

    switch(btn){
    case MOUSE_LEFT:
        ced_needs_redraw = true;
        gettimeofday(&tv, 0);
        //FIX IT: get the system double click time
        if( (tv.tv_sec*1000000+tv.tv_usec-doubleClickTime) < 300000 && (tv.tv_sec*1000000+tv.tv_usec-doubleClickTime) > 5){ //1000000=1sec

            if(!ced_picking(x,y,&mm.mv.x,&mm.mv.y,&mm.mv.z)){
               int id = SELECTED_ID;
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
          ced_needs_redraw = true;
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

  case 27: // esc
    exit(0);
  case 'c':
  case 'C':
    if (!ced_get_selected(x, y, &mm.mv.x, &mm.mv.y, &mm.mv.z)) {
      ced_needs_redraw = true;
    }
    break;

    SELECT_FROM_MENU('`', LAYER_ALL);

    SELECT_FROM_MENU('~', DETECTOR_ALL);

  case 'z':
    if (setting.detector_cut_z < 7000) {
      setting.detector_cut_z += 100;
    }
    ced_needs_redraw = true;
    break;

  case 'Z':
    if (setting.detector_cut_z > -7000) {
      setting.detector_cut_z -= 100;
    }
    ced_needs_redraw = true;
    break;

  case '<':
    if (setting.detector_trans > 0.005) {
      setting.detector_trans -= 0.005;
    } else {
      setting.detector_trans = 0;
    }
    ced_needs_redraw = true;
    break;

  case '>':
    if (setting.detector_trans < 1 - 0.005) {
      setting.detector_trans += 0.005;
    } else {
      setting.detector_trans = 1.;
    }
    ced_needs_redraw = true;
    break;

  case 'm':
    if (setting.detector_cut_angle > 0) {
      setting.detector_cut_angle -= 0.5;
    }
    ced_needs_redraw = true;
    break;

  case 'M':
    if (setting.detector_cut_angle < 360) {
      setting.detector_cut_angle += 0.5;
    }
    ced_needs_redraw = true;
    break;

  case 'h':
    showHelp = !showHelp;
    ced_needs_redraw = true;
    break;
  default: {
    int layer = layer_from_key(key);
    if (layer >= 0) {
      toggle_layer(layer);
      ced_needs_redraw = true;
    } else {
      std::cerr << "Unknown keyboard shortcut: " << key << std::endl;
    }
  }
  }
}

void SpecialKey( int key, int, int ){
   switch (key) {
   case KEY_RIGHT:
    mm.mv.z+=50.;
    break;
   case KEY_LEFT:
    mm.mv.z-=50.;
    break;

   case KEY_UP:
    mm.mv.y+=50.;
    break;
   case KEY_DOWN:
    mm.mv.y-=50.;
    break;

   default:
      return;
   }
   ced_needs_redraw = true;
}


void motion(int x,int y){
    if((move_mode == NO_MOVE) || !window_width || !window_height)
      return;

    if(move_mode == TURN_XY){
      mm.ha=mm.ha_start+(x-mouse_x)*180./window_width;
      mm.va=mm.va_start+(y-mouse_y)*180./window_height;

      //todo
    } else if (move_mode == ORIGIN){
        float grad2rad=M_PI*2/360;
        float x_factor_x =  cos(mm.ha*grad2rad);
        float x_factor_y =  cos((mm.va-90)*grad2rad)*cos((mm.ha+90)*grad2rad);
        float y_factor_x =  0;
        float y_factor_y = -cos(mm.va*grad2rad);
        float z_factor_x =  cos((mm.ha-90)*grad2rad);
        float z_factor_y = -cos(mm.ha*grad2rad)*cos((mm.va+90)*grad2rad);

        float scale_factor=580/mm.sf/exp(log(window_width*window_height)/2.5) ;


        mm.mv.x=mm.mv_start.x- scale_factor*(x-mouse_x)*x_factor_x - scale_factor*(y-mouse_y)*x_factor_y;
        mm.mv.y=mm.mv_start.y- scale_factor*(x-mouse_x)*y_factor_x - scale_factor*(y-mouse_y)*y_factor_y;
        mm.mv.z=mm.mv_start.z -scale_factor*(x-mouse_x)*z_factor_x - scale_factor*(y-mouse_y)*z_factor_y;
    }
    ced_needs_redraw = true;
}
