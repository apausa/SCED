#include "../draw.h"
#include "ui/selection.h"

/*
 * Hit element
 */

void ced_draw_hit(CED_Hit *h){
    GLfloat d;
    float x = h->p.x;
    float y = h->p.y;
    float z = h->p.z;

    if(setting.phi_projection){
        //phi_projection is on
        y = y > 0 ? sqrt(x*x + y*y) : -1*sqrt(x*x + y*y);
        x = 0; 
    }
   
    if(setting.z_projection){
        z=0;
    }

    if(!IS_VISIBLE(h->layer)){
        return;
    }


    ced_color(h->color);



    glDisable(GL_BLEND);

    switch(h->type){

    	case CED_HIT_CROSS:
    	case CED_HIT_BOX:
    	case CED_HIT_VXD:
    	case CED_HIT_STAR:
            glLineWidth(1.);

    	    glBegin(GL_LINES);

    	    if(h->type ==  CED_HIT_CROSS){
    	        d=((GLfloat)h->size)/20.;
    	        glVertex3f(x-d,y-d,z);
    	        glVertex3f(x+d,y+d,z);
		glVertex3f(x+d,y-d,z);
    	        glVertex3f(x-d,y+d,z);       
            }
    	    else if(h->type ==  CED_HIT_STAR){
    	        d=((GLfloat)h->size)/20.;

    	        glVertex3f(x-d,y,z);
    	        glVertex3f(x+d,y,z);
    
		glVertex3f(x,y-d,z);
    	        glVertex3f(x,y+d,z);

		glVertex3f(x,y,z-d);
    	        glVertex3f(x,y,z+d);
	    }
    	    else if(h->type ==  CED_HIT_VXD){
                d=0.005;
#if 1
    	        glVertex3f(x-d,y-d,z+d);
    	        glVertex3f(x+d,y+d,z-d);
    
    	        glVertex3f(x+d,y-d,z+d);
    	        glVertex3f(x-d,y+d,z-d);
    
    	        glVertex3f(x+d,y+d,z+d);
    	        glVertex3f(x-d,y-d,z-d);
    
    	        glVertex3f(x-d,y+d,z+d);
    	        glVertex3f(x+d,y-d,z-d);
#endif
    	    } else {
    	        d=((GLfloat)h->size)/20.;
    	        glVertex3f(x-d,y,z);
    	        glVertex3f(x+d,y,z);
    	        glVertex3f(x,y-d,z);
    	        glVertex3f(x,y+d,z);
    	        glVertex3f(x,y,z-d);
    	        glVertex3f(x,y,z+d); 
    	    }
            glEnd();
    	    break;
    	default:
    	    glPointSize((GLfloat)h->size);
    	    glBegin(GL_POINTS);

            glVertex3f(x,y,z);
            glEnd();

    }

    glEnable(GL_BLEND);
    ced_add_objmap(&h->p,5,h->lcioID,h->layer,0);
}
