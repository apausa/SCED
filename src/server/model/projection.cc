#include "camera.h"
#include "projection.h"
#include "settings.h"

extern CEDsettings setting;

static double cut_angle_backup;
static double cut_z_backup;
static float ha_backup;
static float va_backup;
static int persp_backup;

static bool persp_flipped;

static void flip_persp(void){
    setting.persp = !setting.persp;
    persp_flipped = true;
}

void projection_toggle_phi(void){
    if(setting.phi_projection){ //turn projection off
        setting.phi_projection=false;

        setting.detector_cut_angle=cut_angle_backup;
        if(persp_backup != setting.persp){flip_persp(); } //restore persp setting

        camera_set_angles(ha_backup, va_backup);

        setting.fixed_view=false;

    }else{ //turn projection on
        if(setting.z_projection){
            projection_toggle_z();
        }

        cut_angle_backup=setting.detector_cut_angle;

        setting.phi_projection=true;

        persp_backup=setting.persp;

        if(setting.persp==1){flip_persp(); }


        setting.detector_cut_angle=180;
        ha_backup=camera_get().ha;
        va_backup=camera_get().va;
        camera_set_angles(90., 0.);

        setting.fixed_view=true;
    }
}

void projection_toggle_z(void){
    if(setting.z_projection){ //turn projection off

        setting.z_projection=false;

        setting.detector_cut_z=cut_z_backup;
        if(persp_backup != setting.persp){flip_persp(); } //restore persp setting


        camera_set_angles(ha_backup, va_backup);

        setting.fixed_view=false;
    }else{ //turn projection on

        if(setting.phi_projection){projection_toggle_phi();}

        cut_z_backup=setting.detector_cut_z;

        setting.z_projection=true;

        setting.detector_cut_z=-10;

        persp_backup=setting.persp;

        if(setting.persp==true){flip_persp(); }


       //side view
        ha_backup=camera_get().ha;
        va_backup=camera_get().va;

        camera_set_angles(180., 0.);

        setting.fixed_view=true;
    }
}

void projection_view_front(void){
    if(setting.fixed_view){ return;}

    camera_set_angles(180., 0.);
}

void projection_view_side(void){
    if(setting.fixed_view){ return;}

    camera_set_angles(90., 0.);
}

void projection_set_persp(bool on){
    if(setting.persp != on){
        flip_persp();
    }
}

bool projection_take_persp_changed(void){
    bool changed = persp_flipped;
    persp_flipped = false;
    return changed;
}

void projection_clear(void){
    setting.phi_projection=false;
    setting.z_projection=false;
    setting.fixed_view=false;
}
