/****************************************************************
NAME:
    actions.h
DESCRIPTION:
    Action-ID constants dispatched by selectFromMenu() (see
    src/server/controller/actions.cc), the shared handler for keyboard
    shortcuts and the scroll wheel. The keymap (keymap.cc) binds keys to
    these ids.
****************************************************************/

#pragma once

void selectFromMenu(int id);

#define VIEW_FRONT      21
#define VIEW_SIDE       22
#define VIEW_ZOOM_IN    23
#define VIEW_ZOOM_OUT   24
#define VIEW_RESET      25
#define CED_RESET       27

#define LAYER_ALL       60


#define DETECTOR_ALL            4100

#define TOGGLE_PHI_PROJECTION   5000
#define TOGGLE_Z_PROJECTION     5001

// Bound to keys like the ones above, but handled by keypressed() itself.
#define VIEW_CENTER             30
#define HELP_TOGGLE             31
#define DETECTOR_CUT_Z_UP       32
#define DETECTOR_CUT_Z_DOWN     33
#define DETECTOR_TRANS_UP       34
#define DETECTOR_TRANS_DOWN     35
#define DETECTOR_CUT_ANGLE_UP   36
#define DETECTOR_CUT_ANGLE_DOWN 37
