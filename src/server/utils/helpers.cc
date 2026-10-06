#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <sced_types.h>
#include <settings.h>

#include "utils/helpers.h"

extern CEDsettings setting;
extern float WORLD_SIZE;
extern GLfloat axe[][3];
extern int showHelp;
extern bool ced_needs_redraw;

// allows to reset the visible world size
void set_world_size( float length) {
  WORLD_SIZE = length ;
  axe[1][0] = WORLD_SIZE / 2. ;
  axe[2][1] = WORLD_SIZE / 2. ;
  axe[3][2] = WORLD_SIZE / 2. ;
};

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
