#pragma once

#include <sced_types.h>

extern const char layer_keys[];
extern const char detec_layer_keys[];

int layer_from_key(unsigned char key);

void print_layer_text(CED_TEXT *obj);
