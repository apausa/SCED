#pragma once

/*
 * The pose of the camera. The controller changes it through the functions below,
 * the view reads it through camera_get().
 */

typedef struct {
    float x;
    float y;
    float z;
} Point;

struct CameraState {
    float va; // vertical angle
    float ha; // horisontal angle
    float sf; // scale factor
    Point mv; // the center
    float va_start; // the pose when the current drag began
    float ha_start;
    Point mv_start;
};

const CameraState &camera_get(void);

// Back to the pose the program starts with.
void camera_reset(void);

// One step is 50 pixels of the window height.
void camera_zoom_in(float window_height);
void camera_zoom_out(float window_height);

void camera_set_angles(float ha, float va);
void camera_center_on(float x, float y, float z);
void camera_shift_y(double d);
void camera_shift_z(double d);

// Start of a mouse drag: remember the pose that camera_rotate() and camera_pan() work from.
void camera_drag_begin(void);

// Turn by dha / dva degrees from the pose at the start of the drag.
void camera_rotate(double dha, double dva);

// Move the centre from its position at the start of the drag; dx and dy are the pointer movement in pixels.
void camera_pan(float dx, float dy, float window_width, float window_height);
