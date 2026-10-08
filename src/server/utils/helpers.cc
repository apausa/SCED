#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <sced_types.h>
#include <settings.h>

#include "utils/helpers.h"

extern CEDsettings setting;
extern int showHelp;
extern bool ced_needs_redraw;

int isLayerVisible(int x){
    //return(ced_visible_layers[x]);

    return(setting.layer[x]);
}

void toggleHelpWindow(void){ //hauke
    if(showHelp == 1){
        showHelp=0;
    }else{
        showHelp=1;
    }
    ced_needs_redraw = true;
}
