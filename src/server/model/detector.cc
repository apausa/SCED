#include "detector.h"

#define TRANSPARENCY_STEP 0.01  // how much transparency changes
#define WEDGE_ANGLE_STEP 1      // how much the wedge angle changes, in degrees
#define CUT_Z_STEP 100          // how much the cut z changes

// Detector variables
static double transparency;
static double wedge_angle;
static double cut_z;

// Detector variable setters
double detector_get_transparency(void) { return transparency; }
double detector_get_wedge_angle(void) { return wedge_angle; }
double detector_get_cut_z(void) { return cut_z; }

// Detector variable getters
void detector_set_transparency(double t) { transparency = t; }
void detector_set_wedge_angle(double w) { wedge_angle = w; }
void detector_set_cut_z(double z) { cut_z = z; }

// Reset all detector variables
void detector_reset(void) {
  const double default_transparency = 0.75;
  const double default_wedge_angle = 0;  // degrees
  const double default_cut_z = 7000;     // @fix maximum detector length

  detector_set_transparency(default_transparency);
  detector_set_wedge_angle(default_wedge_angle);
  detector_set_cut_z(default_cut_z);
}

// Increase detector transparency
void detector_transparency_up(void) {
  const double max_transparency = 1;  // invisible

  if (transparency < max_transparency - TRANSPARENCY_STEP) {
    detector_set_transparency(transparency + TRANSPARENCY_STEP);
  } else {
    detector_set_transparency(max_transparency);
  }
}

// Decrease detector transparency
void detector_transparency_down(void) {
  const double min_transparency = 0;  // opaque

  if (transparency > min_transparency + TRANSPARENCY_STEP) {
    detector_set_transparency(transparency - TRANSPARENCY_STEP);
  } else {
    detector_set_transparency(min_transparency);
  }
}

// Widen the wedge cut out of the detector
void detector_wedge_angle_up(void) {
  const double max_wedge_angle = 360;  // degrees
  if (wedge_angle < max_wedge_angle) {
    detector_set_wedge_angle(wedge_angle + WEDGE_ANGLE_STEP);
  }
}

// Narrow the wedge cut out of the detector
void detector_wedge_angle_down(void) {
  const double min_wedge_angle = 0;  // degrees

  if (wedge_angle > min_wedge_angle) {
    detector_set_wedge_angle(wedge_angle - WEDGE_ANGLE_STEP);
  }
}

// Increase detector cut along z axis
void detector_cut_z_up(void) {
  const double max_cut_z = 7000;

  if (cut_z < max_cut_z) {
    detector_set_cut_z(cut_z + CUT_Z_STEP);
  }
}

// Decrease detector cut along z axis
void detector_cut_z_down(void) {
  const double min_cut_z = -7000;

  if (cut_z > min_cut_z) {
    detector_set_cut_z(cut_z - CUT_Z_STEP);
  }
}
