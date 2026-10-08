#pragma once

#include <sced_types.h>

extern char layerDescription[CED_MAX_LAYER][CED_MAX_LAYER_CHAR];
extern const char layer_keys[];
extern const char detec_layer_keys[];

int layer_from_key(unsigned char key);

void addLayerDescriptionToMenu(int id, char *str);
void print_layer_text(CED_TEXT *obj);
