#pragma once

#include <string>

static constexpr int CED_TITLE_BAR_HEIGHT = 24;

// One row of the general shortcuts in the help frame, shown as "[key] description".
struct ShortcutHelp {
    const char *key;
    const char *description;
};

// The controller sets the shortcuts once at start-up, so the view needs nothing from the controller.
void overlay_set_shortcuts(
    const ShortcutHelp *rows,
    int count,
    const char *data_layer_keys,
    const char *detector_layer_keys
);

void printFPS(void);
void printShortcuts(void);
void draw_ced_title_bar(void);

std::string truncateTo(std::string str, size_t max_len);
