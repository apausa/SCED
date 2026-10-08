#include <cstring>

#include <sced_types.h>

#include <model/layers.h>

#include "layers.h"

// Key that toggles each layer: the position in the string is the layer number.
// Detector layers come after the NUMBER_DATA_LAYER data layers.
const char layer_keys[] = "0123456789)!@#$%^&*(tyuio";
const char detec_layer_keys[] = "jkl;'p[]\\TYUIOP{}|aeABDEGHJKLNQWXdgnqw:.";

static_assert(sizeof(layer_keys) - 1 == NUMBER_DATA_LAYER, "one key per data layer");
static_assert(sizeof(detec_layer_keys) - 1 == NUMBER_DETECTOR_LAYER, "one key per detector layer");

int layer_from_key(unsigned char key){
    if(key == 0){ return -1; }

    if(const char *p = strchr(layer_keys, key)){
        return p - layer_keys;
    }
    if(const char *p = strchr(detec_layer_keys, key)){
        return NUMBER_DATA_LAYER + (p - detec_layer_keys);
    }
    return -1;
}

void print_layer_text(CED_TEXT *obj){
    layer_set_description(obj->id, obj->text);
}
