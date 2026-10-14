#pragma once

// How the detector is drawn. The view reads these.
double detector_trans(void);      // how opaque the faces are, 0 .. 1
double detector_cut_angle(void);  // degrees of the wedge cut out of the tubes
double detector_cut_z(void);      // the z at which the detector is cut

// Used by the projections, which force a cut and restore it afterwards.
void detector_set_cut_angle(double angle);
void detector_set_cut_z(double z);

void detector_cut_z_up(void);
void detector_cut_z_down(void);
void detector_trans_up(void);
void detector_trans_down(void);
void detector_cut_angle_up(void);
void detector_cut_angle_down(void);

// The start-up appearance: transparency 0.8, cut angle 0, cut z 7000.
void detector_reset(void);
