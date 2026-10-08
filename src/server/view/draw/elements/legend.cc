#include <cstdio>
#include <cstring>

#include "../draw.h"
#include "third_party/gl_font.h"

// Draws the energy spectrum legend
void ced_draw_legend(CED_Legend *legend){
    //saves the matrices on the stack
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    //changes the matrices to be compatible with the old ced_draw_legend code:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    GLfloat w = (GLfloat)viewport[2];
    GLfloat h = (GLfloat)viewport[3];

    glOrtho(0,w,h,0, -1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    //begin original code:

	int color_steps = legend->color_steps;
	float ene_max = legend->ene_max;
	float ene_min = legend->ene_min;
	unsigned int ticks = legend->ticks;
	char scale = legend->scale;
	++ticks; // incremented so that input value is only the number of 'middle ticks'

	/*
	 * The legend position, width and height */
	float legendThickness = 24;
	float legendHeight = 256;
	float stripeThickness = legendHeight/(float)color_steps;
	float x_min = w-104; // anchored to the right edge, leaves room for the tick labels
	float x_max = x_min+legendThickness;
	float y_min = 64+legendHeight; // bottom of the bar, below the title bar
	float y_max = y_min-stripeThickness;

	int tickNumber = 1; // 'middle' tick counter
	int i;

	int font = setting.font;

	/** ticks */
	char string[6];
	int x_offset = 40;
	float y_offset = (stripeThickness+font_get_height(font))/2;
	float num;

	/** Legend header */
	char header [] = "GeV";
	char footer [] = "LOG";
	int x_offset_legend = 48;
	int y_offset_legend = font_get_height(font)/2;

  	int tick_size = 8;

	/**
	 *  Legend header: GeV */
    double dark=1.0-(setting.bgcolor[0]+setting.bgcolor[1]+setting.bgcolor[2])/3.0; //ever readable color
    glColor3f(dark,dark,dark);

	font_render(font, x_min-x_offset_legend, y_min-stripeThickness*color_steps-y_offset_legend, header);
	glEnd();

	/**
	 *  Legend footer: LOG or LIN */
	switch(scale){
		case 'a': default:
			font_render(font, x_min-x_offset_legend, y_min-y_offset_legend, footer);
			glEnd();
		break;
		/** LIN */
		case 'b':
			strncpy( footer, "LIN", 4 );
			font_render(font, x_min-x_offset_legend, y_min-y_offset_legend, footer);
			glEnd();
		break;
	}

	for (i=0; i<color_steps; ++i) {
		/** This draws the colour spectrum */
		glColor3f(legend->rgb_matrix[i][0]/(float)color_steps,legend->rgb_matrix[i][1]/(float)color_steps,legend->rgb_matrix[i][2]/(float)color_steps);

		glBegin(GL_POLYGON);
		glRasterPos2f(x_min, y_min);
		glVertex3f( x_min,y_min-stripeThickness*i,0.0);
		glVertex3f( x_max,y_min-stripeThickness*i,0.0);
		glVertex3f( x_max,y_max-stripeThickness*i,0.0);
		glVertex3f( x_min,y_max-stripeThickness*i,0.0);
		glEnd();

		/**
		 * Legend: Max & min value display */
		if (i==0 || i==(color_steps-1)){
			glBegin(GL_POLYGON);
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(x_min, y_min);
			glVertex3f( x_max,y_min-stripeThickness*i,0.0);
			glVertex3f( x_max+tick_size,y_min-stripeThickness*i,0.0);
			glVertex3f( x_max+tick_size,y_max-stripeThickness*i,0.0);
			glVertex3f( x_max,y_max-stripeThickness*i,0.0);
			glEnd();

			/**
		 	 * Spectrum max & min value display */
            glColor3f(dark,dark,dark);



			if (i==0){
				snprintf(string, 6,  "%.1f", ene_min);
				font_render(font, x_min+x_offset, y_min-y_offset, string);
			}
			else if (i==(color_steps-1)){
				snprintf(string, 6, "%.1f", ene_max);
				font_render(font, x_min+x_offset, y_min-stripeThickness*i-y_offset, string);
            }
		}

		/**
		 *  Legend: middle ticks */
		else if ((i%((color_steps-1)/ticks))==0 && (unsigned)tickNumber<ticks){

			float pos;
			pos = (float)tickNumber*(float)color_steps/(float)ticks;

			glBegin(GL_POLYGON);
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(x_min, y_min);
			glVertex3f( x_max,y_min-stripeThickness*pos,0.0);
			glVertex3f( x_max+tick_size,y_min-stripeThickness*pos,0.0);
			glVertex3f( x_max+tick_size,y_max-stripeThickness*pos,0.0);
			glVertex3f( x_max,y_max-stripeThickness*pos,0.0);
			glEnd();

			/** Mid-tick legend generation: LOG */
			switch(scale){
				case 'a': default:
					num = pow( (ene_max +1)/(ene_min +1), (float)tickNumber/(float)ticks ) * (ene_min+1) - 1;
				break;
				/** LIN */
				case 'b':
					num = (((ene_max-ene_min)/ticks)*tickNumber) + ene_min;
				break;
			}

			snprintf(string, 6, "%.1f", num);


            glColor3f(dark,dark,dark);
			font_render(font, x_min+x_offset, y_min-stripeThickness*pos-y_offset, string);

			++tickNumber;
		}
	}
	glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
