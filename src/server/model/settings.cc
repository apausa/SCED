#include <iostream>

#include <sced_types.h>

#include "layers.h"
#include "settings.h"

CEDsettings setting;

void defaultSettings(void){
    setting.persp=true;
    setting.antia=true;

    setting.show_axes=true;

    layers_show_all();

    setting.detector_trans=0.8;
    setting.detector_cut_angle=0;
    setting.detector_cut_z=7000;

    setting.phi_projection=false;
    setting.z_projection=false;
    setting.fixed_view=false;

    std::cout << "Set options to default settings" << std::endl;
}
