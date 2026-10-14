/* Action dispatch for CED's event display server.
 * Split out of glced.cc: this is the handler invoked for every
 * keyboard shortcut and scroll-wheel action. */

#include <model/settings.h>
#include <model/camera.h>
#include <model/detector.h>
#include <model/layers.h>
#include <model/projection.h>

#include "actions.h"
#include "view/view_state.h"

// reshape() builds the projection, so the view has to run it again when the perspective changes.
static void reshape_if_persp_changed(void){
    if(projection_take_persp_changed()){
        window_reshape();
    }
}

void selectFromMenu(int id){
    switch(id){
        case VIEW_RESET:
            projection_clear();
            camera_reset();
            break;

        case CED_RESET:
            projection_set_persp(true); //switch persp on
            reshape_if_persp_changed();
            detector_reset();

            layers_show_all();
            projection_clear();
            camera_reset();
            break;

        case VIEW_FRONT:
            projection_view_front();
            break;

        case VIEW_SIDE:
            projection_view_side();
            break;

        case TOGGLE_PHI_PROJECTION:
            projection_toggle_phi();
            reshape_if_persp_changed();
            break;

        case TOGGLE_Z_PROJECTION:
            projection_toggle_z();
            reshape_if_persp_changed();
            break;

        case VIEW_ZOOM_IN:
            camera_zoom_in(get_window_size().h);
            break;

        case VIEW_ZOOM_OUT:
            camera_zoom_out(get_window_size().h);
            break;

        case LAYER_ALL:
            layers_toggle_all_data();
            break;

        case DETECTOR_ALL:
            layers_toggle_all_detector();
            break;
    }

    view_request_redraw();

}
