#include <iostream>

#include "detector.h"
#include "layers.h"
#include "projection.h"
#include "settings.h"

void defaultSettings(void){
    layers_show_all();

    detector_reset();

    projection_reset();

    std::cout << "Set options to default settings" << std::endl;
}
