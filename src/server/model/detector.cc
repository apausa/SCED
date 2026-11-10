#include "detector.h"

#define TRANSPARENCY_STEP 0.01  // how much transparency changes
#define WEDGE_ANGLE_STEP 1  // how much the wedge angle changes, in degrees
#define CUT_Z_STEP 100  // how much the cut z changes, in mm
#define INITIAL_TRANSPARENCY 0.75  // transparency value at start-up
#define INITIAL_WEDGE_ANGLE 0  // wedge angle at start-up, in degrees
#define INITIAL_CUT_Z 7000  // cut z at start-up, in mm

static const DetectorState detector_initial = {
    INITIAL_TRANSPARENCY,
    INITIAL_WEDGE_ANGLE,
    INITIAL_CUT_Z,  // @fix may not atch the maximum detector length
};

static DetectorState detector_state = detector_initial;

const DetectorState& detector_get(void) { return detector_state; }

// Detector variable setters
void detector_set_transparency(double t) { detector_state.transparency = t; }
void detector_set_wedge_angle(double w) { detector_state.wedge_angle = w; }
void detector_set_cut_z(double z) { detector_state.cut_z = z; }

// Reset the detector state
void detector_reset(void) { detector_state = detector_initial; }

// Increase detector transparency
void detector_transparency_up(void) {
  const double max_transparency = 1;  // invisible

  if (detector_state.transparency < max_transparency - TRANSPARENCY_STEP) {
    detector_set_transparency(detector_state.transparency + TRANSPARENCY_STEP);
  } else {
    detector_set_transparency(max_transparency);
  }
}

// Decrease detector transparency
void detector_transparency_down(void) {
  const double min_transparency = 0;  // opaque

  if (detector_state.transparency > min_transparency + TRANSPARENCY_STEP) {
    detector_set_transparency(detector_state.transparency - TRANSPARENCY_STEP);
  } else {
    detector_set_transparency(min_transparency);
  }
}

// Widen the wedge cut out of the detector
void detector_wedge_angle_up(void) {
  const double max_wedge_angle = 360;  // degrees
  if (detector_state.wedge_angle < max_wedge_angle) {
    detector_set_wedge_angle(detector_state.wedge_angle + WEDGE_ANGLE_STEP);
  }
}

// Narrow the wedge cut out of the detector
void detector_wedge_angle_down(void) {
  const double min_wedge_angle = 0;  // degrees

  if (detector_state.wedge_angle > min_wedge_angle) {
    detector_set_wedge_angle(detector_state.wedge_angle - WEDGE_ANGLE_STEP);
  }
}

// Increase detector cut along z axis
void detector_cut_z_up(void) {
  const double max_cut_z = 7000;

  if (detector_state.cut_z < max_cut_z) {
    detector_set_cut_z(detector_state.cut_z + CUT_Z_STEP);
  }
}

// Decrease detector cut along z axis
void detector_cut_z_down(void) {
  const double min_cut_z = -7000;

  if (detector_state.cut_z > min_cut_z) {
    detector_set_cut_z(detector_state.cut_z - CUT_Z_STEP);
  }
}
