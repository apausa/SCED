/****************************************************************
NAME:
    actions.h
DESCRIPTION:
    Action-ID constants dispatched by selectFromMenu() (see
    src/server/controller/actions.cc), the shared handler for keyboard
    shortcuts and the scroll wheel.
****************************************************************/

#pragma once

void selectFromMenu(int id);

#define GRAFIC_PERSP                 2002

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
