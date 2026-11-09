#pragma once

// Camera state:
// - The controller changes it through the functions below
// - The view reads it through camera_get()

typedef struct {
  float x;
  float y;
  float z;
} Point;

struct CameraState {
  float va;      // vertical angle
  float ha;      // horizontal angle
  Point center;  // the center
  float sf;      // scale factor
};

// Camera state getters
const CameraState& camera_get(void);

// Camera state setters
void camera_set_horizontal_angle(float angle);
void camera_set_vertical_angle(float angle);
void camera_set_scale_factor(float factor);
void camera_set_center(float x, float y, float z);

// Reset the camera state
void camera_reset_state(void);

// Save current camera state
void camera_begin_drag(void);

// Increase the scale factor
void camera_zoom_in(float window_height);

// Decrease the scale factor
void camera_zoom_out(float window_height);

// Move center along the y axis
void camera_shift_y(double d);

// Move center along the z axis
void camera_shift_z(double d);

// Rotate view
void camera_rotate_view(double dha, double dva);

// Drag center
void camera_drag_center(float dx, float dy, float window_width, float window_height);
