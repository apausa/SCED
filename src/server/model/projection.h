#pragma once

/*
 * The front and side views and the two flat projections: phi (the detector unrolled
 * onto one plane) and z (seen along z). A projection switches the perspective off and
 * fixes the view, and switching it off restores both. Nothing here redraws or reshapes
 * the window: after a call that can change the perspective, the caller asks
 * projection_take_persp_changed() and reshapes the view if it says true.
 */

void projection_view_front(void);
void projection_view_side(void);

void projection_toggle_phi(void);
void projection_toggle_z(void);

// Turn perspective on or off.
void projection_set_persp(bool on);

// True if the perspective was flipped since the last call; clears the answer.
bool projection_take_persp_changed(void);

// Both projections off and the view free again. Does not restore the perspective or the cuts.
void projection_clear(void);
