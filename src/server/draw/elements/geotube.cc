#include "../../third_party/fg_geometry.h"

#include "../draw.h"
#include <config.h>

void ced_draw_geotube(CED_GeoTube *c){
    using namespace std;
    if(!IS_VISIBLE(c->type)){
        return;
    }

    double transformed_shift = c->shift;


    double cut_angle=setting.detector_cut_angle;
    double trans_value=setting.detector_trans;

    double cut_z=setting.detector_cut_z;


    double d_o = c->r_o;
    double d_i = c->r_i;

    double z0 = transformed_shift;
    double z1 = c->z+c->shift;
    double z = z1-z0;

   if(-1*cut_z > (transformed_shift+2*z)){
        //component is completly in outside range
      return; 
   }else if(-1*cut_z > (transformed_shift)){
      //some part of component is in cutting range
      double backup_transformed_shift=transformed_shift;
      transformed_shift=transformed_shift+(-1*cut_z-transformed_shift)/2.;
      z=z-(-1*cut_z-transformed_shift);
      transformed_shift=backup_transformed_shift+(-1*cut_z-backup_transformed_shift);
   }



    glPushMatrix();
    for(int k=0;k<2;k++){        

        GLfloat face_color[4]={static_cast<GLfloat>(((c->color>>16)&0xff)/255.0),static_cast<GLfloat>(((c->color>>8)&0xff)/255.0),static_cast<GLfloat>(((c->color)&0xff)/255.0), static_cast<GLfloat>(trans_value)};


        float detector_lines_wide=CED_GEOTUBE_LINE_WIDTH;
        

        glGetDoublev(GL_COLOR_CLEAR_VALUE, setting.bgcolor);

        GLfloat line_color[4]={static_cast<GLfloat>(((c->color>>16)&0xff)/255.0/2.0+(1.0-setting.bgcolor[0])/2.0),static_cast<GLfloat>(((c->color>>8)&0xff)/255.0/2.0+(1.0-setting.bgcolor[1])/2.0),static_cast<GLfloat>(((c->color)&0xff)/255.0/2.0+(1.0-setting.bgcolor[2])/2.0), static_cast<GLfloat>((1-trans_value)+CED_GEOTUBE_LINE_MAX_TRANS)}; //lines in detector color mixed with anti background color


        glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA); //locks nice, but lines diapear, so switch it off after drawing

        glMatrixMode(GL_MODELVIEW);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        

        if(cut_angle < 360){
            glTranslatef(0.0, 0.0, transformed_shift);
            if(c->rotate_o > 0.01 ) glRotatef(c->rotate_o, 0, 0, 1);

            if(c->rotate_o <= cut_angle){ //dont cut if rotate angle is to big
                if(c->edges_o != c->edges_i || c->rotate_i != 0){
                    glColor4f(face_color[0], face_color[1], face_color[2], face_color[3]);
                    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);


                    //here
                    //draw the inner shape
                    glRotatef(c->rotate_i, 0, 0, 1);
 
                    glPolygonOffset( 1.f, 1.f );
                    if(trans_value < 1.0){
                        if(k==0)
                            drawPartialCylinder(z*2, d_o-(d_o-d_i)/5, d_i, c->edges_i, cut_angle - c->rotate_i- c->rotate_o, c->rotate_i + c->rotate_o,0,1); //draw the inner cylinder 
                    }

                    glRotatef(-1*c->rotate_i, 0, 0, 1);

                    //draw the outer shape
                    glPolygonOffset( 1.f, 1.f ); 
                    glEnable( GL_POLYGON_OFFSET_FILL );
                    glPolygonOffset( 2.f, 2.f );
                    if(trans_value < 1.0){
                        if(k==0)
                            drawPartialCylinder(z*2, d_o, d_i+(d_o-d_i)/5, c->edges_o, cut_angle - c->rotate_o, c->rotate_o,1,0, c->rotate_i); //draw the outer cylinder
                    }

                    glLineWidth(detector_lines_wide);
                    glColor4f(line_color[0], line_color[1], line_color[2], line_color[3]);
                    glRotatef(c->rotate_i, 0, 0, 1);
                    //draw the inner cylinder 
                    if(k==1)
                    drawPartialLineCylinder(z*2, d_o-(d_o-d_i)/5, d_i, c->edges_i, cut_angle - c->rotate_i- c->rotate_o, c->rotate_i + c->rotate_o,0,1); 
                    glRotatef(-1*c->rotate_i, 0, 0, 1);
                    //draw the outer cylinder

                    if(k==1)
                    drawPartialLineCylinder(z*2, d_o, d_i+(d_o-d_i)/5, c->edges_o, cut_angle - c->rotate_o, c->rotate_o,1,0);
                }else{
                    glColor4f(face_color[0], face_color[1], face_color[2], face_color[3]);

                    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

                    if(trans_value < 1.0){
                        if(k==0)
                            drawPartialCylinder(z*2, d_o, d_i, c->edges_o, cut_angle - c->rotate_o, c->rotate_o);
                    }

                    glLineWidth(detector_lines_wide);


                    glLineWidth(detector_lines_wide);
                    glColor4f(line_color[0], line_color[1], line_color[2], line_color[3]);

                    if(k==1)
                    drawPartialLineCylinder(z*2, d_o, d_i, c->edges_o, cut_angle - c->rotate_o, c->rotate_o);
                }
            }else{
                if(c->edges_o != c->edges_i || c->rotate_i != 0){
                    glColor4f(face_color[0], face_color[1], face_color[2], face_color[3]);

                    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);





                    //draw the inner shape
                    glRotatef(c->rotate_i, 0, 0, 1);
 
                    glPolygonOffset( 1.f, 1.f );
                    if(trans_value < 1.0){
                        if(k==0)
                            drawPartialCylinder(z*2, d_o-(d_o-d_i)/5, d_i, c->edges_i, 0,0 ,0,1); //draw the inner cylinder 
                    }

                    glRotatef(-1*c->rotate_i, 0, 0, 1);


                    //draw the outer shape
                    glPolygonOffset( 1.f, 1.f ); 
                    glEnable( GL_POLYGON_OFFSET_FILL );
                    glPolygonOffset( 2.f, 2.f );
                    if(trans_value < 1.0){
                        if(k==0)
                        drawPartialCylinder(z*2, d_o, d_i+(d_o-d_i)/5, c->edges_o, 0,0,1,0, c->rotate_i); //draw the outer cylinder
                    }



                    glLineWidth(detector_lines_wide);


                    glLineWidth(detector_lines_wide);
                    glColor4f(line_color[0], line_color[1], line_color[2], line_color[3]);



                    glRotatef(c->rotate_i, 0, 0, 1);
                    //draw the inner cylinder 

                    if(k==1)
                    drawPartialLineCylinder(z*2, d_o-(d_o-d_i)/5, d_i, c->edges_i, 0,0,0,1); 
                    glRotatef(-1*c->rotate_i, 0, 0, 1);
                    //draw the outer cylinder

                    if(k==1)
                    drawPartialLineCylinder(z*2, d_o, d_i+(d_o-d_i)/5, c->edges_o, 0,0,1,0);
                }else{
                    glColor4f(face_color[0], face_color[1], face_color[2], face_color[3]);

                    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

                    if(trans_value < 1.0){
                        if(k==0)
                        drawPartialCylinder(z*2, d_o, d_i, c->edges_o, 0,0);
                    }

                    glLineWidth(detector_lines_wide);


                    glColor4f(line_color[0], line_color[1], line_color[2], line_color[3]);
                    glLineWidth(detector_lines_wide);
                    glColor4f(line_color[0], line_color[1], line_color[2], line_color[3]);

                    if(k==1)
                        drawPartialLineCylinder(z*2, d_o, d_i, c->edges_o, 0,0);
                }
            }

            glTranslatef(0.0, 0.0, -transformed_shift);
            glRotatef(-1*c->rotate_o, 0, 0, 1);

        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //default
    }
    glPopMatrix();
}
