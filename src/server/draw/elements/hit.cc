#include <SDL3/SDL.h>

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

    // time is passed to the hit data and is expected to be animated
    bool to_animate = h->time > 0.f;
    if ( to_animate && animate_layer == -1 ) animation_start_time = (int)SDL_GetTicks();

    if(!IS_VISIBLE(h->layer)){
        if (to_animate && animate_layer == int(h->layer) ) animate_layer = -1;
        return;
    }

    if (to_animate){
        if ( animate_layer == -1 ) animate_layer = h->layer;
        else if ( animate_layer != int(h->layer) ){
            setting.layer[animate_layer] = false;
            animate_layer = h->layer;
            animation_start_time = (int)SDL_GetTicks();
        }
        float elapsed_time = 0.001*( (int)SDL_GetTicks() - animation_start_time); // in seconds
        if ( elapsed_time < h->time ) return ;
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
