#pragma once

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <math.h>

#include <sced_types.h>
#include <model/settings.h>

extern CEDsettings setting;

#define IS_VISIBLE(x) ((x < CED_MAX_LAYER && (int)x >= 0)?setting.layer[x]:false)

// Register the element types with their draw functions. The order must be
// the same as in the client (see sced_elements.h).
void ced_register_elements(void);

// Shared helpers
void ced_color(unsigned rgba);

// Draw a partial cylinder made of lines (for the detector geometry)
void drawPartialLineCylinder(
    double length,
    double R /*radius*/,
    double iR /*inner radius*/,
    int edges,
    double angle_cut_off,
    double angle_cut_off_left,
    bool outer_face=1,
    bool inner_face=1
);

// Draw a partial cylinder made of planes (for the detector geometry)
void drawPartialCylinder(double length,
    double R /*radius*/,
    double iR /*inner radius*/,
    int edges,
    double angle_cut_off,
    double angle_cut_off_left,
    bool outer_face=1,
    bool inner_face=1,
    double irotate=0
);

// Draw functions
void ced_draw_hit(CED_Hit *h);
void ced_draw_line(CED_Line *h);
void ced_draw_geotube(CED_GeoTube *c);
void ced_draw_geocylinder(CED_GeoCylinder *c);
void ced_draw_geocylinder_r(CED_GeoCylinderR *c);
void ced_draw_ellipsoid_r(CED_EllipsoidR *eli);
void ced_draw_cluellipse_r(CED_CluEllipseR *eli);
void ced_draw_geobox(CED_GeoBox *box);
void ced_draw_geobox_r(CED_GeoBoxR *box);
void ced_draw_cone_r(CED_ConeR *cone);
void ced_draw_geobox_r_solid(CED_GeoBoxR *box);
void ced_draw_legend(CED_Legend *legend);
