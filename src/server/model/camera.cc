#include <math.h>

#include "camera.h"

#define INITIAL_VERTICAL_ANGLE 30.     // vertical angle at start-up, in degrees
#define INITIAL_HORIZONTAL_ANGLE 150.  // horizontal angle at start-up, in degrees
#define INITIAL_SCALE_FACTOR 0.1       // scale factor at start-up, zoomed out
#define INITIAL_PERSPECTIVE true       // perspective on at start-up
#define ZOOM_STEP 50.0                 // how much zoom changes, in pixels

static const CameraState camera_initial = {
    INITIAL_VERTICAL_ANGLE, INITIAL_HORIZONTAL_ANGLE, {0., 0., 0.},
    INITIAL_SCALE_FACTOR,   INITIAL_PERSPECTIVE,
};

static CameraState camera_state = camera_initial;

static CameraState dragged_state;

static bool perspective_change;

// Camera state getter
const CameraState& camera_get(void) { return camera_state; }

// Camera state setters
void camera_set_horizontal_angle(float angle) { camera_state.ha = angle; }
void camera_set_vertical_angle(float angle) { camera_state.va = angle; }
void camera_set_scale_factor(float factor) { camera_state.sf = factor; }
void camera_set_center(float x, float y, float z) {
  camera_state.center.x = x;
  camera_state.center.y = y;
  camera_state.center.z = z;
}
void camera_set_perspective(bool perspective) {
  if (camera_state.perspective != perspective) {  // If new perspective differs
    camera_state.perspective = perspective;
    perspective_change = true;  // Flag change in perspective
  }
}

// Reset the camera state
void camera_reset_state(void) {
  camera_set_perspective(camera_initial.perspective);  // Flag potential change in the perspective
  camera_state = camera_initial;
}

// Save current camera state
void camera_begin_drag(void) { dragged_state = camera_state; }

// Increase the scale factor
void camera_zoom_in(float window_height) {
  camera_set_scale_factor(camera_state.sf + camera_state.sf * ZOOM_STEP / window_height);
}

// Decrease the scale factor
void camera_zoom_out(float window_height) {
  camera_set_scale_factor(camera_state.sf - camera_state.sf * ZOOM_STEP / window_height);
}

// Move center along the y axis
void camera_shift_y(double d) { camera_state.center.y += d; }

// Move center along the z axis
void camera_shift_z(double d) { camera_state.center.z += d; }

// Rotate view
void camera_rotate_view(double dha, double dva) {
  camera_set_horizontal_angle(dragged_state.ha + dha);
  camera_set_vertical_angle(dragged_state.va + dva);
}

// Drag center
void camera_drag_center(float dx, float dy, float window_width, float window_height) {
  float to_rad = M_PI * 2 / 360;
  float scale_factor = 580 / camera_state.sf / exp(log(window_width * window_height) / 2.5);

  float x_per_dx = cos(camera_state.ha * to_rad);
  float x_per_dy = cos((camera_state.va - 90) * to_rad) * cos((camera_state.ha + 90) * to_rad);
  float y_per_dx = 0;
  float y_per_dy = -cos(camera_state.va * to_rad);
  float z_per_dx = cos((camera_state.ha - 90) * to_rad);
  float z_per_dy = -cos(camera_state.ha * to_rad) * cos((camera_state.va + 90) * to_rad);

  float x = dragged_state.center.x - scale_factor * dx * x_per_dx - scale_factor * dy * x_per_dy;
  float y = dragged_state.center.y - scale_factor * dx * y_per_dx - scale_factor * dy * y_per_dy;
  float z = dragged_state.center.z - scale_factor * dx * z_per_dx - scale_factor * dy * z_per_dy;

  camera_set_center(x, y, z);
}

// Check change in perspective to rebuild the projection
bool camera_check_perspective(void) {
  bool status = false;

  if (perspective_change) {  // If change is flagged
    status = perspective_change;
    perspective_change = false;  // Reset flag
  }

  return status;
}
