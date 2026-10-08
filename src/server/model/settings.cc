#include <iostream>

#include <sced_types.h>
#include <third_party/gl_font.h>

#include "layers.h"
#include "settings.h"

CEDsettings setting;

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

    layers_show_all();

    setting.detector_trans=0.8;
    setting.detector_cut_angle=0;
    setting.detector_cut_z=7000;

    setting.phi_projection=false;
    setting.z_projection=false;
    setting.fixed_view=false;

    std::cout << "Set options to default settings" << std::endl;
}
