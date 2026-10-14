#include "../draw.h"
#include <model/layers.h>
#include "view/pick.h"

/*
 * Line element
 */

void ced_draw_line(CED_Line *h){
    if(!layer_visible(h->type)){
        return;
    }

    CED_Point p0 = h->p0;
    CED_Point p1 = h->p1;

  	ced_color(h->color);

  	glLineWidth(h->width);
  	glBegin(GL_LINES);


    
#if 1
    if(settings_get().phi_projection){
      //phi_projection is on
        p0.y = p0.y > 0 ? sqrt(p0.x*p0.x + p0.y*p0.y) : -1*sqrt(p0.x*p0.x + p0.y*p0.y);
        p0.x = 0;

        p1.y = p1.y > 0 ? sqrt(p1.x*p1.x + p1.y*p1.y) : -1*sqrt(p1.x*p1.x + p1.y*p1.y);
        p1.x = 0;
   }
   if(settings_get().z_projection){
    p0.z=0;
    p1.z=0;
   }

    glVertex3fv(&p0.x);
    glVertex3fv(&p1.x);

#endif

  	glEnd();

    ced_add_objmap(&h->p0,5,h->lcioID);
    ced_add_objmap(&h->p1,5,h->lcioID);
}
