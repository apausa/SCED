#pragma once

#include <sced_types.h>

/*
 * Global display/view state for the glced server
 * (grid vs. surface view, projection, per-layer visibility/cuts, etc).
 */
struct CEDsettings{
    bool persp;         //perspectivic view or flat projection
    bool antia;         //anti aliasing
    double detector_trans;
    double detector_cut_angle;
    double detector_cut_z;
    bool layer[CED_MAX_LAYER];
    bool phi_projection;
    bool z_projection;
    bool fixed_view;
    int win_h; //height of the window (pixel)
    int win_w; //wight of the window (pixel)
    double zoom;
    double bgcolor[4];
    bool show_axes;
    int font; //size of text (menu, shortcuts, text in ced window)
};

extern int animation_start_time; // in ms
extern int animate_layer;
