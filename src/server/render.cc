/* OpenGL rendering for CED's event display server.
 * Split out of glced.cc: window/GL-state init, the per-frame draw,
 * and viewport/projection setup. */

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL3/SDL.h>

#include <sced_types.h>
#include <config.h>
#include <settings.h>

#include "third_party/fg_geometry.h"
#include "ui/input.h"
#include "ui/overlay.h"
#include "ui/selection.h"
#include "event.h"

#include "render.h"

extern int showHelp;

extern SDL_Window *ced_sdl_window;

extern GLfloat window_width;
extern GLfloat window_height;

extern Point pick_point;
extern Point pre_pick_point;
extern int selected_layer;
extern bool select_nothing;
extern CEDsettings setting;

void init(void){
    //Set background color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white

    glShadeModel(GL_SMOOTH);

    glClearDepth(1);

    glEnable(GL_DEPTH_TEST); //activate 'depth-test'

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //default

    glEnableClientState(GL_VERTEX_ARRAY);
    // GL_NORMAL_ARRAY GL_COLOR_ARRAY GL_TEXTURE_COORD_ARRAY,GL_EDGE_FLAG_ARRAY

    // to put text
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

// bitmaps for X,Y and Z
static unsigned char x_bm[]={
    0xc3,0x42,0x66,0x24,0x24,0x18,
    0x18,0x24,0x24,0x66,0x42,0xc3
};
static unsigned char y_bm[]={
    0xc0,0x40,0x60,0x20,0x30,0x10,
    0x18,0x2c,0x24,0x66,0x42,0xc3
};
static unsigned char z_bm[]={
    0xff,0x40,0x60,0x20,0x30,0x10,
    0x08,0x0c,0x04,0x06,0x02,0xff
};


static const GLfloat axe[][3]={
  { 0., 0., 0., },
  { WORLD_SIZE/2, 0., 0. },
  { 0., WORLD_SIZE/2, 0. },
  { 0., 0., WORLD_SIZE/2 }
};

static void axe_arrow(void){
    GLfloat k=WORLD_SIZE/window_height;
    geoSolidCone(8.*k, 30.*k, 16, 5);
}

static void display_world(void){
    if(setting.show_axes == false){
        return;
    }


    glColor3f(AXES_COLOR);
    glLineWidth(AXES_LINE_SIZE);

    glBegin(GL_LINES);
    glVertex3fv(axe[0]);
    glVertex3fv(axe[1]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3fv(axe[0]);
    glVertex3fv(axe[2]);
    glEnd();
    glBegin(GL_LINES);
    glVertex3fv(axe[0]);
    glVertex3fv(axe[3]);
    glEnd();

    glColor3f(0.5,0.5,0.8);
    glPushMatrix();
    glTranslatef(WORLD_SIZE/2.-WORLD_SIZE/100.,0.,0.);
    glRotatef(90.,0.0,1.0,0.0);
    axe_arrow();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.,WORLD_SIZE/2.-WORLD_SIZE/100.,0.);
    glRotatef(-90.,1.0,0.,0.);
    axe_arrow();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.,0.,WORLD_SIZE/2.-WORLD_SIZE/100.);
    axe_arrow();
    glPopMatrix();

    // Draw X,Y,Z ...

    glGetDoublev(GL_COLOR_CLEAR_VALUE, setting.bgcolor);
    double dark=1.0-(setting.bgcolor[0]+setting.bgcolor[1]+setting.bgcolor[2])/3.0;
    glColor3f(dark,dark,dark);


    glRasterPos3f(WORLD_SIZE/2.+WORLD_SIZE/8,0.,0.);
    glBitmap(8,12,4,6,0,0,x_bm);
    glRasterPos3f(0.,WORLD_SIZE/2.+WORLD_SIZE/8,0.);
    glBitmap(8,12,4,6,0,0,y_bm);
    glRasterPos3f(0.,0.,WORLD_SIZE/2.+WORLD_SIZE/8);
    glBitmap(8,12,4,6,0,0,z_bm);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    setting.zoom=mm.sf;
    glScalef(mm.sf,mm.sf,mm.sf); //zoom

    glRotatef(mm.va,1.,0.,0.); //rotate
    glRotatef(mm.ha,0.,1.0,0.); //rotate
    glTranslatef(-mm.mv.x,-mm.mv.y,-mm.mv.z); //move

    if(setting.picking_highlight==true && select_nothing == false){
        glColor3f(1,0,0);
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex3f(pick_point.x,pick_point.y,pick_point.z);
        glEnd();
    }
    // draw static objects
    display_world(); //only axes?

    // draw elements (hits + detector)
    ced_prepare_objmap();
    ced_do_draw_event();


    if(showHelp == 1){
        printShortcuts();
    }


    draw_ced_title_bar();
    printFPS();
    printEventTime();

    SDL_GL_SwapWindow(ced_sdl_window);

    glPopMatrix();
}

void reshape(int w,int h){
    window_width=w;
    window_height=h;
    setting.win_w=w;
    setting.win_h=h;

    if(setting.antia){
        glEnable (GL_LINE_SMOOTH);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    }else{
        glDisable(GL_POINT_SMOOTH);
        glDisable(GL_LINE_SMOOTH);
    }

    if(setting.persp == false){

        glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-WORLD_SIZE*w/h,WORLD_SIZE*w/h,-WORLD_SIZE,WORLD_SIZE, -15*WORLD_SIZE,15*WORLD_SIZE);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }else{
        glViewport(0,0,w,h);

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glLoadMatrixf(glm::value_ptr(glm::perspective(
            glm::radians((GLfloat)CAMERA_FIELD_OF_VIEW),
            window_width/window_height,
            (GLfloat)CAMERA_MIN_DISTANCE,
            (GLfloat)(CAMERA_MAX_DISTANCE)
        )));

        glMatrixMode( GL_MODELVIEW );

        glLoadIdentity();

        glMultMatrixf(glm::value_ptr(glm::lookAt(
            glm::vec3(CAMERA_POSITION),
            glm::vec3(0,0,0),
            glm::vec3(0,1,0)
        )));
    }
}
