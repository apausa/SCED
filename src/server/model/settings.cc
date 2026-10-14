#include <iostream>

#include <sced_types.h>

#include "detector.h"
#include "layers.h"
#include "projection.h"
#include "settings.h"

static CEDsettings setting;

const CEDsettings &settings_get(void){
    return setting;
}

CEDsettings &settings_edit(void){
    return setting;
}

void defaultSettings(void){
    setting.antia=true;

    setting.show_axes=true;

    layers_show_all();

    detector_reset();

    projection_reset();

    std::cout << "Set options to default settings" << std::endl;
}
