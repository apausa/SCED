/* Action dispatch for CED's event display server.
 * Split out of glced.cc: this is the handler invoked for every
 * keyboard shortcut and scroll-wheel action. */

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <sced_types.h>
#include <model/settings.h>

#include "actions.h"
#include <model/camera.h>
#include <model/layers.h>
#include <model/projection.h>

#include "controller/input.h"
#include "view/view.h"

extern GLfloat window_width;
extern GLfloat window_height;
extern bool ced_needs_redraw;
extern CEDsettings setting;

// Hack: call the resize function to overwrite the perspective settings.
static void reshape_view(void){
    reshape((int)window_width, (int)window_height);
}

void selectFromMenu(int id){
    switch(id){
        case VIEW_RESET:
            projection_clear();
            camera_reset();
            break;

        case CED_RESET:
            projection_set_persp(true); //switch persp on
            if(projection_take_persp_changed()){
                reshape_view();
            }
            setting.detector_trans=0.8;
            setting.detector_cut_angle=0;
            setting.detector_cut_z=7000;

            layers_show_all();
            projection_clear();
            camera_reset();

            setting.show_axes=true;
            break;

        case VIEW_FRONT:
            projection_view_front();
            break;

        case VIEW_SIDE:
            projection_view_side();
            break;

        case TOGGLE_PHI_PROJECTION:
            projection_toggle_phi();
            if(projection_take_persp_changed()){
                reshape_view();
            }
            break;

        case TOGGLE_Z_PROJECTION:
            projection_toggle_z();
            if(projection_take_persp_changed()){
                reshape_view();
            }
            break;

        case VIEW_ZOOM_IN:
            camera_zoom_in(window_height);
            break;

        case VIEW_ZOOM_OUT:
            camera_zoom_out(window_height);
            break;

        case LAYER_ALL:
            layers_toggle_all_data();
            break;

        case DETECTOR_ALL:
            layers_toggle_all_detector();
            break;
    }

    ced_needs_redraw = true;

}
