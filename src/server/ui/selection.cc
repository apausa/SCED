#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include <sced_types.h>
#include <settings.h>

#include "selection.h"

extern CEDsettings setting;

int SELECTED_ID = -1;

/*
 * To support mouse operations with objects, we need
 * object screen coordinates.
 */
static GLdouble modelM[16];
static GLdouble projM[16];
static GLint    viewport[4];

typedef struct {
    unsigned int ID;
    unsigned int layer;
    unsigned int type;
    int x; // in window
    int y;
    int z;
    int max_dxy; // after this distance, ignore this object
    CED_Point p; // object real coordinates (can't use pointer...)
} CED_ObjMap;

static CED_ObjMap *omap=0;
static unsigned omap_count=0;
static unsigned omap_alloced=0;

/*
 * To be called from drawing functions
 */
void ced_add_objmap(CED_Point *p,int max_dxy, unsigned int ID, unsigned int layer, int type){
    double my_max_dxy =  5*max_dxy*setting.zoom;

    GLdouble winx,winy,winz;

    if(omap_count==omap_alloced){
        omap_alloced+=4096;
        omap=(CED_ObjMap*) realloc(omap,omap_alloced*sizeof(CED_ObjMap));
        if ( omap == NULL ) {
          std::cout << "glced will exit because of a failure to allocate memory in ced_add_objmap..." << std::endl;
          exit(1);
        }

    }
    {
        glm::dvec3 win = glm::project(
            glm::dvec3(p->x, p->y, p->z),
            glm::make_mat4(modelM),
            glm::make_mat4(projM),
            glm::dvec4(viewport[0], viewport[1], viewport[2], viewport[3])
        );

        winx = win.x;
        winy = win.y;
        winz = win.z;
    }
    omap[omap_count].ID=ID;
    omap[omap_count].type=type;
    omap[omap_count].layer=layer;
    omap[omap_count].x=(int)winx;
    omap[omap_count].y=(int)winy;
    omap[omap_count].z=(int)(1000. * winz);
    omap[omap_count].max_dxy=int(my_max_dxy+0.5);
    omap[omap_count++].p=*p;
}

/*
 * Fill matrixes with current world coordinates
 * !!! Must be called just before ced_do_draw_event() !!!
 */
void ced_prepare_objmap(void){
    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,modelM);
    glGetDoublev(GL_PROJECTION_MATRIX,projM);

    omap_count=0;
}

/*
 * If return zero, will set World coordinates.
 *
 * !!! There is some danger that objects will change between
 *     last drawing and this function call...
 */
int ced_get_selected(int x,int y,GLfloat *wx,GLfloat *wy,GLfloat *wz){
    CED_ObjMap *p,*best;
    unsigned i;
    int dx,dy;
    int d,dist=0; // calculate dist as |x-x'|+|y-y'|
    
    y=viewport[3]-y-1; // to get correct direction
    for(i=0,p=omap,best=0;i<omap_count;i++,p++){
        dx=abs(p->x-x);
        dy=abs(p->y-y);
        if((dx>p->max_dxy) || (dy>p->max_dxy)){
            continue;
        }
        d=(int) pow(pow(dx,2)+pow(dy,2),0.5);
        if(!best || (d<dist)){
            best=p;
            dist=d;
        }
    }
    if(!best){
        return 1;
    }
    *wx=best->p.x;
    *wy=best->p.y;
    *wz=best->p.z;

    std::cout << "Center selected hit (ID: " << best->ID << ")" << std::endl;

    SELECTED_ID = best->ID;
    return 0;
}

int find_selected_object(int x,int y,GLfloat *wx,GLfloat *wy,GLfloat *wz, int *id, int *layer, int *type){
    CED_ObjMap *p,*best;
    unsigned i;
    int dx,dy;
    int d,dist=0; // calculate dist as |x-x'|+|y-y'|
    
    y=viewport[3]-y-1; // to get correct direction
    for(i=0,p=omap,best=0;i<omap_count;i++,p++){
        dx=abs(p->x-x);
        dy=abs(p->y-y);
        if((dx>p->max_dxy) || (dy>p->max_dxy)){
            continue;
        }
        
        d=(int) pow(pow(dx,2)+pow(dy,2),0.5);
        if(!best || (d<dist)){
            best=p;
            dist=d;
        }
    }
    if(!best){
        return 1;
    }

    //new
    if(best->type==1){
        for(i=0,p=omap,best=0;i<omap_count;i++,p++){
            dx=abs(p->x-x);
            dy=abs(p->y-y);
            if((dx>p->max_dxy) || (dy>p->max_dxy)){
                continue;
            }
            
            d=(int) pow(pow(dx,2)+pow(dy,2)+pow(p->z/5.,2),0.5);
            if(!best || (d<dist)){
                best=p;
                dist=d;
            }
        }
    }
    //end new
    *wx=best->p.x;
    *wy=best->p.y;
    *wz=best->p.z;

    *id = best->ID;
    *layer = best->layer;
    *type = best->type;
    return 0;
}


/*************************************************************** 
* A extra picking function, do the same as ced_get_selected,   *
* without center the selected object                           *
***************************************************************/
int ced_picking(int x,int y,GLfloat*, GLfloat*, GLfloat*){
    CED_ObjMap *p,*best;
    unsigned i;
    int dx,dy;
    int d,dist=0; // calculate dist as |x-x'|+|y-y'|

    y=viewport[3]-y-1; // to get correct direction
    for(i=0,p=omap,best=0;i<omap_count;i++,p++){
        dx=abs(p->x-x);
        dy=abs(p->y-y);
        if((dx>p->max_dxy) || (dy>p->max_dxy)){
            continue;
        }
        d=(int) pow(pow(dx,2)+pow(dy,2),0.5);
        if(!best || (d<dist)){
            best=p;
            dist=d;
        }
    }
    if(!best){
        SELECTED_ID =0;
        return 1;
    }
    printf("Picking: HIT %d\n",best->ID);

    SELECTED_ID = best->ID;
    return 0;
}

