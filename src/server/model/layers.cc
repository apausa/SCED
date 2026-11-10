#include <cstdio>
#include <cstring>

#include <sced_types.h>
#include "layers.h"

// Global variables

static bool layerVisibility[CED_MAX_LAYER];
static char layerDescription[CED_MAX_LAYER][CED_MAX_LAYER_CHAR];

// Getter functions

bool layers_get_visibility(int layer) {
  return layer >= 0 && layer < CED_MAX_LAYER && layerVisibility[layer];
}

const char* layers_get_description(int layer) {
  return (layer < 0 || layer >= CED_MAX_LAYER) ? "" : layerDescription[layer];
}

// Setter functions

void layers_set_description(int id, const char* text) {
  if (id < 0 || id >= CED_MAX_LAYER) {
    printf("Warning: Layer id out of range\n");
    return;
  }
  strncpy(layerDescription[id], text, CED_MAX_LAYER_CHAR - 1);
}

void layers_set_visibility(unsigned l) {
  if (l > CED_MAX_LAYER - 1) return;

  layerVisibility[l] = !layerVisibility[l];
}

// Remaining functions, called by the controller to change the state

void layers_reset(void) {
  for (int i = 0; i < CED_MAX_LAYER; i++) {
    layerVisibility[i] = true;
  }
}

static void toggle_all(int first, int end) {
  int count = 0;

  // First if some layers are hidden, it shows them all
  for (int i = first; i < end; i++) {
    if (!layerVisibility[i]) {
      layers_set_visibility(i);
      count++;
    }
  }

  // Else if all layers are shown, it hides them all (and viceversa)
  if (count == 0) {
    for (int i = first; i < end; i++) {
      layers_set_visibility(i);
    }
  }
}

void layers_toggle_all_data(void) { toggle_all(0, NUMBER_DATA_LAYER); }

void layers_toggle_all_detector(void) {
  toggle_all(NUMBER_DATA_LAYER, NUMBER_DETECTOR_LAYER + NUMBER_DATA_LAYER);
}

// @fix only first 64 keys have a layer assigned (can be toggled)
