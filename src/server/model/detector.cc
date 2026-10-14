#include "detector.h"

static double trans;
static double cut_angle;
static double cut_z;

double detector_trans(void){
    return trans;
}

double detector_cut_angle(void){
    return cut_angle;
}

double detector_cut_z(void){
    return cut_z;
}

void detector_set_cut_angle(double angle){
    cut_angle=angle;
}

void detector_set_cut_z(double z){
    cut_z=z;
}

void detector_reset(void){
    trans=0.8;
    cut_angle=0;
    cut_z=7000;
}

void detector_cut_z_up(void){
    if (cut_z < 7000) {
      cut_z += 100;
    }
}

void detector_cut_z_down(void){
    if (cut_z > -7000) {
      cut_z -= 100;
    }
}

void detector_trans_down(void){
    if (trans > 0.005) {
      trans -= 0.005;
    } else {
      trans = 0;
    }
}

void detector_trans_up(void){
    if (trans < 1 - 0.005) {
      trans += 0.005;
    } else {
      trans = 1.;
    }
}

void detector_cut_angle_down(void){
    if (cut_angle > 0) {
      cut_angle -= 0.5;
    }
}

void detector_cut_angle_up(void){
    if (cut_angle < 360) {
      cut_angle += 0.5;
    }
}
