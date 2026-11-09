#pragma once

// Detector variable getters
double detector_get_transparency(void);
double detector_get_wedge_angle(void);
double detector_get_cut_z(void);

// Detector variable setters
void detector_set_transparency(double t);
void detector_set_wedge_angle(double angle);
void detector_set_cut_z(double z);

// Reset all detector variables
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
