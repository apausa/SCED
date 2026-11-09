#include "detector.h"

// Smallest wedge, in degrees
#define WEDGE_MIN 0

// Largest wedge, in degrees (all tubes cut away)
#define WEDGE_MAX 360

// How much wedge changes, in degrees
#define WEDGE_STEP 1

static double transparency;
static double wedge;
static double cut_z;

double detector_get_transparency(void) { return transparency; }

double detector_get_wedge(void) { return wedge; }

double detector_get_cut_z(void) { return cut_z; }

void detector_set_transparency(double t) { transparency = t; }

void detector_set_wedge(double w) { wedge = w; }

void detector_set_cut_z(double z) { cut_z = z; }

void detector_reset(void) {
  detector_set_transparency(0.8);
  detector_set_wedge(WEDGE_MIN);
  detector_set_cut_z(7000);
}

void detector_cut_z_up(void) {
  if (cut_z < 7000) {
    detector_set_cut_z(cut_z + 100);
  }
}

void detector_cut_z_down(void) {
  if (cut_z > -7000) {
    detector_set_cut_z(cut_z - 100);
  }
}

void detector_transparency_down(void) {
  if (transparency > 0.005) {
    detector_set_transparency(transparency - 0.005);
  } else {
    detector_set_transparency(0);
  }
}

void detector_transparency_up(void) {
  if (transparency < 1 - 0.005) {
    detector_set_transparency(transparency + 0.005);
  } else {
    detector_set_transparency(1.);
  }
}

void detector_wedge_down(void) {
  if (wedge > WEDGE_MIN) {
    detector_set_wedge(wedge - WEDGE_STEP);
  }
}

void detector_wedge_up(void) {
  if (wedge < WEDGE_MAX) {
    detector_set_wedge(wedge + WEDGE_STEP);
  }
}
