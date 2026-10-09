#include "detector.h"
#include "settings.h"

extern CEDsettings setting;

void detector_cut_z_up(void){
    if (setting.detector_cut_z < 7000) {
      setting.detector_cut_z += 100;
    }
}

void detector_cut_z_down(void){
    if (setting.detector_cut_z > -7000) {
      setting.detector_cut_z -= 100;
    }
}

void detector_trans_down(void){
    if (setting.detector_trans > 0.005) {
      setting.detector_trans -= 0.005;
    } else {
      setting.detector_trans = 0;
    }
}

void detector_trans_up(void){
    if (setting.detector_trans < 1 - 0.005) {
      setting.detector_trans += 0.005;
    } else {
      setting.detector_trans = 1.;
    }
}

void detector_cut_angle_down(void){
    if (setting.detector_cut_angle > 0) {
      setting.detector_cut_angle -= 0.5;
    }
}

void detector_cut_angle_up(void){
    if (setting.detector_cut_angle < 360) {
      setting.detector_cut_angle += 0.5;
    }
}
