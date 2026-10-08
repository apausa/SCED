#pragma once

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <sced_types.h>

/*
 * To be called from drawing functions
 */
void ced_add_objmap(CED_Point *p,int max_dxy, unsigned int ID, unsigned int layer=0, int type=0);

/*
 * Fill matrixes with current world coordinates
 * !!! Must be called just before ced_do_draw_event() !!!
 */
void ced_prepare_objmap(void);

/*
 * If return zero, will set World coordinates.
 *
 * !!! There is some danger that objects will change between
 *     last drawing and this function call...
 */
int ced_get_selected(int x,int y,GLfloat *wx,GLfloat *wy,GLfloat *wz);

/***************************************************************
* A extra picking function, do the same as ced_get_selected,   *
* without center the selected object                           *
***************************************************************/
int ced_picking(int x,int y,GLfloat *wx,GLfloat *wy,GLfloat *wz);

extern int SELECTED_ID;
