#pragma once

/*
 * Which layers are shown and the text that describes each layer. Layers
 * 0 .. NUMBER_DATA_LAYER-1 are data, the NUMBER_DETECTOR_LAYER layers after them
 * are detector. Nothing here requests a redraw.
 */

// Flip the visibility of one layer (0 .. CED_MAX_LAYER-1).
void toggle_layer(unsigned l);

// Turn all data (or detector) layers on; if they are all on already, turn them all off.
void layers_toggle_all_data(void);
void layers_toggle_all_detector(void);

// Every layer on, including those without a key.
void layers_show_all(void);

void layer_set_description(int id, const char *text);
const char *layer_description(int layer);
