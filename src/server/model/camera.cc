#include <math.h>

#include "camera.h"

static const CameraState camera_initial = {
    30.,
    150.,
    0.1, // decrease zoom, set redraw scale a lot smaller
    { 0., 0., 0. },
    0.,
    0.,
    { 0., 0., 0. },
};

static CameraState mm = camera_initial;

const CameraState &camera_get(void){
    return mm;
}

void camera_reset(void){
    mm=camera_initial;
}

void camera_zoom_in(float window_height){
    mm.sf += mm.sf*50.0/window_height;
}

void camera_zoom_out(float window_height){
    mm.sf -= mm.sf*50.0/window_height;
}

void camera_set_angles(float ha, float va){
    mm.ha=ha;
    mm.va=va;
}

void camera_center_on(float x, float y, float z){
    mm.mv.x=x;
    mm.mv.y=y;
    mm.mv.z=z;
}

void camera_shift_y(double d){
    mm.mv.y+=d;
}

void camera_shift_z(double d){
    mm.mv.z+=d;
}

void camera_drag_begin(void){
    mm.ha_start=mm.ha;
    mm.va_start=mm.va;
    mm.mv_start=mm.mv;
}

void camera_rotate(double dha, double dva){
    mm.ha=mm.ha_start+dha;
    mm.va=mm.va_start+dva;
}

void camera_pan(float dx, float dy, float window_width, float window_height){
    float grad2rad=M_PI*2/360;
    float x_factor_x =  cos(mm.ha*grad2rad);
    float x_factor_y =  cos((mm.va-90)*grad2rad)*cos((mm.ha+90)*grad2rad);
    float y_factor_x =  0;
    float y_factor_y = -cos(mm.va*grad2rad);
    float z_factor_x =  cos((mm.ha-90)*grad2rad);
    float z_factor_y = -cos(mm.ha*grad2rad)*cos((mm.va+90)*grad2rad);

    float scale_factor=580/mm.sf/exp(log(window_width*window_height)/2.5) ;

    mm.mv.x=mm.mv_start.x- scale_factor*dx*x_factor_x - scale_factor*dy*x_factor_y;
    mm.mv.y=mm.mv_start.y- scale_factor*dx*y_factor_x - scale_factor*dy*y_factor_y;
    mm.mv.z=mm.mv_start.z -scale_factor*dx*z_factor_x - scale_factor*dy*z_factor_y;
}
