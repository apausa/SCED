#include <cstring>
#include <vector>

#include <sced_types.h>

#include "view/overlay.h"

#include "actions.h"
#include "keymap.h"

struct Shortcut {
    const char *key;
    int action;
    const char *description;
};

// The general shortcuts, in the order the help lists them.
// ESC and the arrow keys have no typed character: event_loop.cc and SpecialKey() handle them.
static const Shortcut shortcuts[] = {
    {"ESC", 0, "Quit CED"},
    {"h", HELP_TOGGLE, "Toggle shortcut frame"},
    {"r", VIEW_RESET, "Reset view"},
    {"R", CED_RESET, "Reset CED"},
    {"f", VIEW_FRONT, "Front view"},
    {"s", VIEW_SIDE, "Side view"},
    {"F", TOGGLE_Z_PROJECTION, "Front projection"},
    {"S", TOGGLE_PHI_PROJECTION, "Side projection"},
    {"+", VIEW_ZOOM_IN, "Zoom in"},
    {"-", VIEW_ZOOM_OUT, "Zoom out"},
    {"c", VIEW_CENTER, "Center"},
    {"C", VIEW_CENTER, nullptr},
    {"Z", DETECTOR_CUT_Z_DOWN, "Cut in z-axe direction"},
    {"z", DETECTOR_CUT_Z_UP, "Cut in -z-axe direction"},
    {">", DETECTOR_TRANS_UP, "Increase transparency"},
    {"<", DETECTOR_TRANS_DOWN, "Decrease transparency"},
    {"M", DETECTOR_CUT_ANGLE_UP, "Increase detector cut angle"},
    {"m", DETECTOR_CUT_ANGLE_DOWN, "Decrease detector cut angle"},
    {"->", 0, "Move in z-direction"},
    {"<-", 0, "Move in -z-direction"},
    {"`", LAYER_ALL, "Toggle all data layers"},
    {"~", DETECTOR_ALL, "Toggle all detector layers"},
};

// Key that toggles each layer: the position in the string is the layer number.
// Detector layers come after the NUMBER_DATA_LAYER data layers.
static const char layer_keys[] = "0123456789)!@#$%^&*(tyuio";
static const char detec_layer_keys[] = "jkl;'p[]\\TYUIOP{}|aeABDEGHJKLNQWXdgnqw:.";

static_assert(sizeof(layer_keys) - 1 == NUMBER_DATA_LAYER, "one key per data layer");
static_assert(sizeof(detec_layer_keys) - 1 == NUMBER_DETECTOR_LAYER, "one key per detector layer");

int keymap_action(unsigned char key){
    for(const Shortcut &s : shortcuts){
        if(s.key[1] == 0 && s.key[0] == key){ return s.action; }
    }
    return 0;
}

void initialize_shortcuts(void){
    std::vector<ShortcutHelp> rows;
    for(const Shortcut &s : shortcuts){
        if(s.description){ rows.push_back({s.key, s.description}); }
    }
    overlay_set_shortcuts(rows.data(), rows.size(), layer_keys, detec_layer_keys);
}

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
