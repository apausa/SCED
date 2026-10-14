#pragma once

// Hands the help text of the shortcuts and the layer keys to the view. Call it once before the first frame.
void initialize_shortcuts(void);

// The action (an id from actions.h) bound to a typed character, or 0 if it is not a general shortcut.
int keymap_action(unsigned char key);

// The layer (0 .. NUMBER_DATA_LAYER + NUMBER_DETECTOR_LAYER - 1) that a typed character toggles, or -1 if it toggles none.
int layer_from_key(unsigned char key);
