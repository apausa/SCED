/* Action dispatch for CED's event display server.
 * Split out of glced.cc: this is the handler invoked for every
 * keyboard shortcut and scroll-wheel action. */

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <cstring>
#include <iostream>

#include <sced_types.h>
#include <model/settings.h>

#include "actions.h"
#include <model/camera.h>
#include <model/layers.h>

#include "controller/input.h"
#include "view/view.h"

using namespace std;

extern GLfloat window_width;
extern GLfloat window_height;
extern bool ced_needs_redraw;
extern CEDsettings setting;

void copySetting(CEDsettings &dest, CEDsettings &source, const char *name){
    if(strcmp(name,"trans")==0){
        dest.detector_trans=source.detector_trans;
    }

    else if(strcmp(name,"cut z")==0){
        dest.detector_cut_z=source.detector_cut_z;
    }

    else if(strcmp(name,"cut angle")==0){
        dest.detector_cut_angle=source.detector_cut_angle;
    }
    else{
        std::cout << "WARNING: unknown settingtype: " << name << std::endl;
    }

}

void selectFromMenu(int id){
    static CEDsettings backup_setting;
    static float mm_ha_backup;
    static float mm_va_backup;
    static int graphic_2_backup;

    switch(id){
        case VIEW_RESET:
            setting.phi_projection = false; // no phi projection
            setting.z_projection=false; // no phi projection;
            camera_reset();
            setting.fixed_view=false;
            break;

        case CED_RESET:
            if(setting.persp == false){
                selectFromMenu(GRAFIC_PERSP); //switch persp on
            }
            setting.detector_trans=0.8;
            setting.detector_cut_angle=0;
            setting.detector_cut_z=7000;

            layers_show_all();
            setting.phi_projection = false; // no phi projection
            setting.z_projection=false; // no phi projection;
            camera_reset();
            setting.fixed_view=false;

            setting.show_axes=true;
            break;

        case VIEW_FRONT:
            if(setting.fixed_view){ break;}

            camera_set_angles(180., 0.);
            break;

        case VIEW_SIDE:
            if(setting.fixed_view){ break;}

                camera_set_angles(90., 0.);

            break;

        case TOGGLE_PHI_PROJECTION:
            if(setting.phi_projection){ //turn projection off
                setting.phi_projection=false;

                copySetting(setting, backup_setting, "cut angle");
                if(graphic_2_backup != setting.persp){selectFromMenu(GRAFIC_PERSP); } //restore persp setting

                camera_set_angles(mm_ha_backup, mm_va_backup);

                setting.fixed_view=false;

            }else{ //turn projection on
                if(setting.z_projection){
                    selectFromMenu(TOGGLE_Z_PROJECTION);
                }

                copySetting(backup_setting, setting, "cut angle");

                setting.phi_projection=true;

                graphic_2_backup=setting.persp;

                if(setting.persp==1){selectFromMenu(GRAFIC_PERSP); }


                setting.detector_cut_angle=180;
                mm_ha_backup=camera_get().ha;
                mm_va_backup=camera_get().va;
                camera_set_angles(90., 0.);

                setting.fixed_view=true;
            }

            break;

        case TOGGLE_Z_PROJECTION:
            if(setting.z_projection){ //turn projection off

                setting.z_projection=false;

                copySetting(setting, backup_setting, "cut z");
                if(graphic_2_backup != setting.persp){selectFromMenu(GRAFIC_PERSP); } //restore persp setting


                camera_set_angles(mm_ha_backup, mm_va_backup);

                setting.fixed_view=false;
            }else{ //turn projection on

                if(setting.phi_projection){selectFromMenu(TOGGLE_PHI_PROJECTION);}

                copySetting(backup_setting, setting, "cut z");

                setting.z_projection=true;

                setting.detector_cut_z=-10;

                graphic_2_backup=setting.persp;

                if(setting.persp==true){selectFromMenu(GRAFIC_PERSP); }


               //side view
                mm_ha_backup=camera_get().ha;
                mm_va_backup=camera_get().va;

                camera_set_angles(180., 0.);

                setting.fixed_view=true;
            }

            break;


        case VIEW_ZOOM_IN:
            camera_zoom_in(window_height);
            break;

        case VIEW_ZOOM_OUT:
            camera_zoom_out(window_height);
            break;

        case LAYER_ALL:
            layers_toggle_all_data();
            break;

        case DETECTOR_ALL:
            layers_toggle_all_detector();
            break;

        case GRAFIC_PERSP:
            if(setting.persp == true){
                setting.persp = false;

                reshape((int)window_width, (int)window_height); //hack, call resize function to overwrite perspectivic settings
            }else{
                setting.persp = true;
                reshape((int)window_width, (int)window_height); //hack, call resize function to overwrite perspectivic settings
            }
            break;

    }

    ced_needs_redraw = true;

}
