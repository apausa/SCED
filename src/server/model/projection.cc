#include "camera.h"
#include "detector.h"
#include "projection.h"

static bool persp;
static bool phi_projection;
static bool z_projection;
static bool fixed_view;

static double cut_angle_backup;
static double cut_z_backup;
static float ha_backup;
static float va_backup;
static int persp_backup;

static bool persp_flipped;

bool projection_persp(void){
    return persp;
}

bool projection_phi(void){
    return phi_projection;
}

bool projection_z(void){
    return z_projection;
}

bool projection_fixed(void){
    return fixed_view;
}

static void flip_persp(void){
    persp = !persp;
    persp_flipped = true;
}

void projection_toggle_phi(void){
    if(phi_projection){ //turn projection off
        phi_projection=false;

        detector_set_cut_angle(cut_angle_backup);
        if(persp_backup != persp){flip_persp(); } //restore persp setting

        camera_set_angles(ha_backup, va_backup);

        fixed_view=false;

    }else{ //turn projection on
        if(z_projection){
            projection_toggle_z();
        }

        cut_angle_backup=detector_cut_angle();

        phi_projection=true;

        persp_backup=persp;

        if(persp==1){flip_persp(); }


        detector_set_cut_angle(180);
        ha_backup=camera_get().ha;
        va_backup=camera_get().va;
        camera_set_angles(90., 0.);

        fixed_view=true;
    }
}

void projection_toggle_z(void){
    if(z_projection){ //turn projection off

        z_projection=false;

        detector_set_cut_z(cut_z_backup);
        if(persp_backup != persp){flip_persp(); } //restore persp setting


        camera_set_angles(ha_backup, va_backup);

        fixed_view=false;
    }else{ //turn projection on

        if(phi_projection){projection_toggle_phi();}

        cut_z_backup=detector_cut_z();

        z_projection=true;

        detector_set_cut_z(-10);

        persp_backup=persp;

        if(persp==true){flip_persp(); }


       //side view
        ha_backup=camera_get().ha;
        va_backup=camera_get().va;

        camera_set_angles(180., 0.);

        fixed_view=true;
    }
}

void projection_view_front(void){
    if(fixed_view){ return;}

    camera_set_angles(180., 0.);
}

void projection_view_side(void){
    if(fixed_view){ return;}

    camera_set_angles(90., 0.);
}

void projection_set_persp(bool on){
    if(persp != on){
        flip_persp();
    }
}

bool projection_take_persp_changed(void){
    bool changed = persp_flipped;
    persp_flipped = false;
    return changed;
}

void projection_clear(void){
    phi_projection=false;
    z_projection=false;
    fixed_view=false;
}

void projection_reset(void){
    persp=true;
    projection_clear();
}
