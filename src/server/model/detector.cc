#include "detector.h"

static double transparency;
static double cut_angle;
static double cut_z;

double detector_get_transparency(void) { return transparency; }

double detector_get_cut_angle(void) { return cut_angle; }

double detector_get_cut_z(void) { return cut_z; }

void detector_set_transparency(double t) { transparency = t; }

void detector_set_cut_angle(double angle) { cut_angle = angle; }

void detector_set_cut_z(double z) { cut_z = z; }

void detector_reset(void) {
  detector_set_transparency(0.8);
  detector_set_cut_angle(0);
  detector_set_cut_z(7000);
}

void detector_cut_z_up(void) {
  if (cut_z < 7000) {
    detector_set_cut_z(cut_z + 100);
  }
}

void detector_cut_z_down(void) {
  if (cut_z > -7000) {
    detector_set_cut_z(cut_z - 100);
  }
}

void detector_transparency_down(void) {
  if (transparency > 0.005) {
    detector_set_transparency(transparency - 0.005);
  } else {
    detector_set_transparency(0);
  }
}

void detector_transparency_up(void) {
  if (transparency < 1 - 0.005) {
    detector_set_transparency(transparency + 0.005);
  } else {
    detector_set_transparency(1.);
  }
}

void detector_cut_angle_down(void) {
  if (cut_angle > 0) {
    detector_set_cut_angle(cut_angle - 0.5);
  }
}

void detector_cut_angle_up(void) {
  if (cut_angle < 360) {
    detector_set_cut_angle(cut_angle + 0.5);
  }
}
