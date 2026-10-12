#include "actions.h"
#include "keymap.h"

struct Shortcut {
    char key;
    int action;
};

// The general shortcuts. The layer keys are in ui/layers.cc.
static const Shortcut shortcuts[] = {
    {'r', VIEW_RESET},
    {'R', CED_RESET},
    {'f', VIEW_FRONT},
    {'F', TOGGLE_Z_PROJECTION},
    {'s', VIEW_SIDE},
    {'S', TOGGLE_PHI_PROJECTION},
    {'+', VIEW_ZOOM_IN},
    {'-', VIEW_ZOOM_OUT},
    {'c', VIEW_CENTER},
    {'C', VIEW_CENTER},
    {'`', LAYER_ALL},
    {'~', DETECTOR_ALL},
    {'z', DETECTOR_CUT_Z_UP},
    {'Z', DETECTOR_CUT_Z_DOWN},
    {'<', DETECTOR_TRANS_DOWN},
    {'>', DETECTOR_TRANS_UP},
    {'m', DETECTOR_CUT_ANGLE_DOWN},
    {'M', DETECTOR_CUT_ANGLE_UP},
    {'h', HELP_TOGGLE},
};

int keymap_action(unsigned char key){
    for(const Shortcut &s : shortcuts){
        if(s.key == key){ return s.action; }
    }
    return 0;
}
