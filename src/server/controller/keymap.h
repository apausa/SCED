#pragma once

// Hands the help text of the shortcuts to the view. Call it once before the first frame.
void initialize_shortcuts(void);

// The action (an id from actions.h) bound to a typed character, or 0 if it is not a general shortcut.
int keymap_action(unsigned char key);
