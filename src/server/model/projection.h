#pragma once

// Projection state:
// - The controller changes it through the functions below
// - The view and controller read it through projection_get()

struct ProjectionState {
  bool phi;  // the detector unrolled onto one plane
  bool z;  // the detector seen along the z axis
};

const ProjectionState& projection_get(void);

// Turn both projections off
void projection_reset(void);

// Change to front view
void projection_view_front(void);

// Change to side view
void projection_view_side(void);

// Toggle phi projection on or off
void projection_toggle_phi(void);

// Toggle z projection on or off
void projection_toggle_z(void);
