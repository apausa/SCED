#pragma once

// Layers getters
const char* layers_get_description(int layer);
bool layers_get_visibility(int layer);

// Layers setters
void layers_set_description(int id, const char* text);
void layers_set_visibility(unsigned l);

// Turn every layer on, including those without a key assigned
void layers_reset(void);

// Show all data layers (from 0 to 24); if they are all on already, turn them all off
void layers_toggle_all_data(void);

// Toggle all detector layers (from 25 to 64); if they are all on already, turn them all off
void layers_toggle_all_detector(void);
