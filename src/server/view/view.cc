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
#include <model/settings.h>
#include <model/camera.h>

#include "third_party/fg_geometry.h"
#include "third_party/gl_font.h"
#include "view/overlay.h"
#include "view/view_state.h"
#include "view/pick.h"
#include "model/event.h"

#include "view.h"

//Color of xyz axes
#define AXES_COLOR                          0.2,0.2,0.8

//Width of xyz axes
#define AXES_LINE_SIZE                      0.5

//Camera field of view, in degree
#define CAMERA_FIELD_OF_VIEW                45

//Camera min distance (hint: min and max should be close together)
#define CAMERA_MIN_DISTANCE                 100

//Camera max distance (hint: min and max should be close together)
static double camera_max_distance(const CameraState &cam){
    return 50000.0*cam.sf+50000/cam.sf;
}

//Where the camera stands
#define CAMERA_POSITION                     0,0,2000

extern CEDsettings setting;

void init(void){
    glEnable(GL_BLEND);

    //Set background color
    const double *bg = get_background_color();
    glClearColor(bg[0], bg[1], bg[2], bg[3]);

    glShadeModel(GL_SMOOTH);

    glClearDepth(1);

    glEnable(GL_DEPTH_TEST); //activate 'depth-test'

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //default

    glEnableClientState(GL_VERTEX_ARRAY);
    // GL_NORMAL_ARRAY GL_COLOR_ARRAY GL_TEXTURE_COORD_ARRAY,GL_EDGE_FLAG_ARRAY

    // to put text
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    font_init();
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
    GLfloat k=WORLD_SIZE/get_window_size().h;
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

    const double *bg = get_background_color();
    double dark=1.0-(bg[0]+bg[1]+bg[2])/3.0;
    glColor3f(dark,dark,dark);


    glRasterPos3f(WORLD_SIZE/2.+WORLD_SIZE/8,0.,0.);
    glBitmap(8,12,4,6,0,0,x_bm);
    glRasterPos3f(0.,WORLD_SIZE/2.+WORLD_SIZE/8,0.);
    glBitmap(8,12,4,6,0,0,y_bm);
    glRasterPos3f(0.,0.,WORLD_SIZE/2.+WORLD_SIZE/8);
    glBitmap(8,12,4,6,0,0,z_bm);
}


// Visits every element type with a draw callback calling it
static void ced_do_draw_event(void){
    const ced_event &ceve = get_event();
    unsigned int i,j;
    ced_element *pe;
    unsigned char *pdata;
    for(i=0;i<ceve.e_count;i++){
        pe=ceve.e+i;
        if(!pe->draw)
            continue;
        for(pdata=pe->b,j=0;j<pe->count;j++,pdata+=pe->size)
            (*(pe->draw))(pdata);
    }
}

void display(void){
    // Smooth points and lines, unless anti-aliasing is off.
    if(setting.antia){
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
    }else{
        glDisable(GL_POINT_SMOOTH);
        glDisable(GL_LINE_SMOOTH);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    const CameraState &cam = camera_get();

    glScalef(cam.sf,cam.sf,cam.sf); //zoom

    glRotatef(cam.va,1.,0.,0.); //rotate
    glRotatef(cam.ha,0.,1.0,0.); //rotate
    glTranslatef(-cam.mv.x,-cam.mv.y,-cam.mv.z); //move

    // draw static objects
    display_world(); //only axes?

    // draw elements (hits + detector)
    ced_prepare_objmap();
    ced_do_draw_event();


    if(view_help_shown()){
        printShortcuts();
    }


    draw_ced_title_bar();
    printFPS();

    glPopMatrix();

    SDL_GL_SwapWindow(view_window());
}

void reshape(int w,int h){
    set_window_size(w, h);

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
            (GLfloat)w/h,
            (GLfloat)CAMERA_MIN_DISTANCE,
            (GLfloat)(camera_max_distance(camera_get()))
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
