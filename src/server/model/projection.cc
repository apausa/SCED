#include "camera.h"
#include "detector.h"
#include "projection.h"

// COnstnat variables

#define INITIAL_PHI false  // phi projection at start-up
#define INITIAL_Z false  // z projection at start-up

// Global variables

static const ProjectionState projection_initial = {INITIAL_PHI, INITIAL_Z};
static ProjectionState projection_state = projection_initial;
static float ha_backup;
static float va_backup;

// Getter functions

const ProjectionState& projection_get(void) { return projection_state; }

// Remaining functions, called by the controller and main to change the state

void projection_reset(void) { projection_state = projection_initial; }

void projection_view_front(void) {
  if (projection_state.phi || projection_state.z) return;

  camera_set_horizontal_angle(180.);
  camera_set_vertical_angle(0.);
}

void projection_view_side(void) {
  if (projection_state.phi || projection_state.z) return;

  camera_set_horizontal_angle(90.);
  camera_set_vertical_angle(0.);
}

void projection_toggle_phi(void) {
  static double wedge_angle_backup;

  if (projection_state.phi) {  // Turn phi projection off
    projection_state.phi = false;

    detector_set_wedge_angle(wedge_angle_backup);  // Restore wedge angle
    camera_set_horizontal_angle(ha_backup);  // Restore horizontal angle
    camera_set_vertical_angle(va_backup);  // Restore vertical angle
    camera_set_perspective(true);  // Turns perspective back on

  } else {  // Turn phi projection on
    if (projection_state.z) projection_toggle_z();

    wedge_angle_backup = detector_get().wedge_angle;  // Store current wedge angle
    ha_backup = camera_get().ha;  // Restore horizontal angle
    va_backup = camera_get().va;  // Restore vertical angle

    camera_set_perspective(false);  // Enable the orthographic projection
    detector_set_wedge_angle(180);  // Cut half of the detector seen from a side view
    projection_view_side();  // Change to side view

    projection_state.phi = true;
  }
}

void projection_toggle_z(void) {
  static double cut_z_backup;

  if (projection_state.z) {  // Turn z projection off
    projection_state.z = false;

    detector_set_cut_z(cut_z_backup);  // Restore z cut
    camera_set_horizontal_angle(ha_backup);  // Restore horizontal angle
    camera_set_vertical_angle(va_backup);  // Restore vertical angle
    camera_set_perspective(true);  // Turn perspective back on

  } else {  // Turn z projection on
    if (projection_state.phi) projection_toggle_phi();

    cut_z_backup = detector_get().cut_z;  // Store current z cut
    ha_backup = camera_get().ha;  // Store current horizontal angle
    va_backup = camera_get().va;  // Store current vertical angle

    camera_set_perspective(false);  // Enable the orthographic projection
    detector_set_cut_z(0);  // Cut half of the detector seen from a front view
    projection_view_front();  // Before updating projection state

    projection_state.z = true;
  }
}
