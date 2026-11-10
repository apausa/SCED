#pragma once

struct DetectorState {
  double transparency;  // transparency levels (0 solid .. 1 invisible)
  double wedge_angle;  // degrees of the wedge cut out
  double cut_z;  // z value at which the detector is cut
};

const DetectorState& detector_get(void);

// Detector state setters
void detector_set_transparency(double t);
void detector_set_wedge_angle(double angle);
void detector_set_cut_z(double z);

// Reset the detector state
void detector_reset(void);

// Make detector more see-through
void detector_transparency_up(void);

// Make detector more opaque
void detector_transparency_down(void);

// Widen the wedge cut out of the detector
void detector_wedge_angle_up(void);

// Narrow the wedge cut out of the detector
void detector_wedge_angle_down(void);

// Increase detector cut along z axis
void detector_cut_z_up(void);

// Decrease detector cut along z axis
void detector_cut_z_down(void);
