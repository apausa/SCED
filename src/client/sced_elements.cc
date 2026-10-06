/* "C" event display.
 * Client side elements definitions. */
#include <string.h>

#include <sced_types.h>
#include "sced_elements.h"
#include <event_buffer.h>
#include <stdio.h>

#include <math.h>


/*
 * Hit element
 */

static unsigned HIT_ID=0;

void ced_hit(float x,float y,float z,unsigned type,unsigned size,unsigned color){
  ced_hit_ID(x,y,z, type & 0xFF,( type >> CED_LAYER_SHIFT ) & 0xFF, size,color, 0);
}

//deprecated
void ced_hit_ID_old(float x,float y,float z,unsigned type, unsigned size,unsigned color, unsigned lcioID){
  ced_hit_ID(x,y,z,type & 0xFF,(type >> CED_LAYER_SHIFT) & 0xFF,size,color,lcioID);
}

void ced_hit_ID(float x,float y,float z,unsigned type,unsigned layer, unsigned size,unsigned color, unsigned lcioID){
 CED_Hit *h=(CED_Hit *)ced_add(HIT_ID);
 if(!h)
   return;
 h->p.x=x;
 h->p.y=y;
 h->p.z=z;
 h->time=0;
 h->type=type;
 h->layer=layer;
 h->size=size;
 h->color=color;
 h->lcioID=lcioID;
}

void ced_hit_ID_animate(float x,float y,float z,float t, unsigned type,unsigned layer, unsigned size,unsigned color, unsigned lcioID){
 CED_Hit *h=(CED_Hit *)ced_add(HIT_ID);
 if(!h) return;
 h->p.x=x;
 h->p.y=y;
 h->p.z=z;
 h->time=t;
 h->type=type;
 h->layer=layer;
 h->size=size;
 h->color=color;
 h->lcioID=lcioID;
}



/*
 * Line element
 */

static unsigned LINE_ID=0;

void ced_line(float x0,float y0,float z0,
	      float x1,float y1,float z1,
	      unsigned type, unsigned width,unsigned color){
    ced_line_ID(x0,y0,z0,x1,y1,z1, type, width, color, 0);
}

void ced_line_ID(float x0,float y0,float z0,
	      float x1,float y1,float z1,
	      unsigned layer, unsigned width,unsigned color, unsigned lcioID){
  CED_Line *l=(CED_Line *)ced_add(LINE_ID);
  if(!l)
    return;
  l->p0.x=x0;
  l->p0.y=y0;
  l->p0.z=z0;
  l->p1.x=x1;
  l->p1.y=y1;
  l->p1.z=z1;
 if(layer > 255){ //downward compability
    l->type=layer >> CED_LAYER_SHIFT;
 }else{
    l->type=layer;
 }
  l->width=width;
  l->color=color;
  l->lcioID=lcioID;
}
/*
 * GeoCylinder
 */
static unsigned GEOC_ID=0;

void ced_geocylinder(float d,unsigned sides,float rotate,float z,float shift,
		     unsigned color){
  CED_GeoCylinder *c=(CED_GeoCylinder *)ced_add(GEOC_ID);
  if(!c)
    return;
  c->d=d;
  c->sides=sides;
  c->rotate=rotate;
  c->z=z;
  c->shift=shift;
  c->color=color;
}

/*
 * Rotated Geocylinder
 * Extension of the cylinder subject to a 3-DOF rotation
 * @date: 26.08.08
 */
static unsigned GEOCR_ID=0;

void ced_geocylinder_r(float d, double z, double * center, double * rotate, unsigned sides, 
		     unsigned int color, int layer){
	int iDim;
  CED_GeoCylinderR *c=(CED_GeoCylinderR *)ced_add(GEOCR_ID);
  if(!c) return;
    for (iDim = 0; iDim < 3; iDim ++ ) {
   		c->center[iDim]  = center[iDim];
   		c->rotate[iDim]   = rotate[iDim];
    }
  c->d=d;
  c->sides=sides;
  c->color=color;
  c->z=z;
 if(layer > 255){ //downward compability
    c->layer=layer >> CED_LAYER_SHIFT;
 }else{
    c->layer=layer;
 }
}
 
void ced_geocylinders(unsigned n,CED_GeoCylinder *all){
  CED_GeoCylinder *c;
  unsigned i;
  for(i=0;i<n;i++){
    c=(CED_GeoCylinder *)ced_add(GEOC_ID);
    if(!c)
      return;
    memcpy(c,all+i,sizeof(CED_GeoCylinder));
  }
}

static unsigned GEOT_ID=0;

void ced_geotubes(unsigned n,CED_GeoTube *all){
  CED_GeoTube *c;
  unsigned i;
  for(i=0;i<n;i++){
    c=(CED_GeoTube *)ced_add(GEOT_ID);
    if(!c)
      return;
    memcpy(c,all+i,sizeof(CED_GeoTube));
  }
}


static unsigned GEOB_ID=0;

void ced_geobox(double * sizes, double * center, unsigned int color ) {
  int iDim;
  CED_GeoBox * box = (CED_GeoBox*) ced_add(GEOB_ID);
  if ( ! box ) return;
  for ( iDim = 0; iDim < 3; iDim ++ ) {
    box->sizes[iDim]   = sizes[iDim];
    box->center[iDim]  = center[iDim];
  }
  box->color   = color;

}

void rotate3d(double *vektor, double *rotate){
    double r_rad[3]={rotate[0]/360*2*3.14159265358979323846, rotate[1]/360*2*3.14159265358979323846, rotate[2]/360*2*3.14159265358979323846};
    double cords1[3] = {vektor[0], vektor[1], vektor[2]};

    vektor[0] = ( cos(r_rad[1])*cos(r_rad[2]) )*cords1[0] + 
                (-cos(r_rad[0])*sin(r_rad[2]) + sin(r_rad[0])*sin(r_rad[1])*cos(r_rad[2]) )*cords1[1] + 
                ( sin(r_rad[0])*sin(r_rad[2]) + cos(r_rad[0])*sin(r_rad[1])*cos(r_rad[2]) )*cords1[2];
    vektor[1] = ( cos(r_rad[1])*sin(r_rad[2]) ) * cords1[0] + 
                ( cos(r_rad[0])*cos(r_rad[2]) + sin(r_rad[0])*sin(r_rad[1])*sin(r_rad[2])) * cords1[1] + 
                ( -sin(r_rad[0])*cos(r_rad[2]) + cos(r_rad[0])*sin(r_rad[1])*sin(r_rad[2]) ) * cords1[2];
    vektor[2] = (-sin(r_rad[1])) * cords1[0] + 
                ( sin(r_rad[0])*cos(r_rad[1])) * cords1[1] + 
                ( cos(r_rad[0])*cos(r_rad[1]))  * cords1[2];
 
}

void ced_geobox_r_ID(double *size, double *position, double *rotate, unsigned int layer, unsigned int color, unsigned int lcio_id) {

    int i;
    double vektor1[3], vektor2[3];

    double cubematrix[12][6] ={ {-1,-1,-1, +1,-1,-1},
                                {-1,-1,-1, -1,+1,-1},
                                {-1,-1,-1, -1,-1,+1},
                                {+1,-1,-1, +1,+1,-1},
                                {+1,-1,-1, +1,-1,+1},
                                {+1,+1,-1, +1,+1,+1},
                                {+1,+1,-1, -1,+1,-1},
                                {+1,+1,+1, -1,+1,+1},
                                {+1,+1,+1, +1,-1,+1},
                                {-1,+1,+1, -1,-1,+1},
                                {-1,+1,+1, -1,+1,-1},
                                {-1,-1,+1, +1,-1,+1} };

    for(i=0;i<12;i++){
        vektor1[0] = cubematrix[i][0]*size[0]/2; 
        vektor1[1] = cubematrix[i][1]*size[1]/2; 
        vektor1[2] = cubematrix[i][2]*size[2]/2;
 
        vektor2[0] = cubematrix[i][3]*size[0]/2;
        vektor2[1] = cubematrix[i][4]*size[1]/2;
        vektor2[2] = cubematrix[i][5]*size[2]/2;

        rotate3d(vektor1,rotate);
        rotate3d(vektor2,rotate);


        ced_line_ID(position[0]+vektor1[0], position[1]+vektor1[1], position[2]+vektor1[2],
                    position[0]+vektor2[0], position[1]+vektor2[1], position[2]+vektor2[2],
                    layer, 1,color, lcio_id);
    }
}

void ced_geobox_ID(double *size, double *position, unsigned int layer, unsigned int color, unsigned int lcio_id) {
    double rotate[3]={0.0, 0.0, 0.0};
    ced_geobox_r_ID(size, position, rotate, layer,  color, lcio_id);
}


void ced_geoboxes(unsigned int nBox, CED_GeoBox * allBoxes ) {
  
  CED_GeoBox * box;
  unsigned int iBox;
  for ( iBox = 0; iBox < nBox ; iBox++ ) {
    box = (CED_GeoBox *) ced_add(GEOB_ID);
    if ( ! box ) return;
    memcpy( box, allBoxes + iBox, sizeof(CED_GeoBox) );
  }
}

static unsigned GEOBR_ID=0;

void ced_geobox_r(double * sizes, double * center, double * rotate, unsigned int color, unsigned int layer) {
  int iDim;
  CED_GeoBoxR * box = (CED_GeoBoxR*) ced_add(GEOBR_ID);
  if ( ! box ) return;
  for ( iDim = 0; iDim < 3; iDim ++ ) {
    box->sizes[iDim]   = sizes[iDim];
    box->center[iDim]  = center[iDim];
    box->rotate[iDim] = rotate[iDim];
  }
  box->color = color;

 if(layer > 255){ //downward compability
    box->layer=layer >> CED_LAYER_SHIFT;
 }else{
    box->layer=layer;
 }
}

static unsigned GEOBRS_ID=0;

void ced_geobox_r_solid(double * sizes, double * center, double * rotate, unsigned int color, unsigned int layer) {
  int iDim;
  CED_GeoBoxR * box = (CED_GeoBoxR*) ced_add(GEOBRS_ID);
  if ( ! box ) return;
  for ( iDim = 0; iDim < 3; iDim ++ ) {
    box->sizes[iDim]   = sizes[iDim];
    box->center[iDim]  = center[iDim];
    box->rotate[iDim] = rotate[iDim];
  }
  box->color = color;

 if(layer > 255){ //downward compability
    box->layer=layer >> CED_LAYER_SHIFT;
 }else{
    box->layer=layer;
 }
}

static unsigned LEGEND_ID=0;

void ced_legend(float ene_min, float ene_max, unsigned int color_steps, unsigned int ** rgb_matrix, unsigned int ticks, char scale) {
	CED_Legend * legend = (CED_Legend*) ced_add(LEGEND_ID);
	if ( ! legend ) return;
	
	legend->ene_min = ene_min;
  	legend->ene_max = ene_max;
 	legend->color_steps = color_steps;
 	legend->ticks = ticks;
 	legend->scale = scale;
 	
  	const unsigned int numberOfColours = 3;
  	unsigned int i,j;
  	for (i = 0; i < numberOfColours; i ++ ) {
  		for (j = 0; j < color_steps; j ++ ) {
  			legend->rgb_matrix[j][i] = rgb_matrix[j][i];
  		}
	}
}

static unsigned TEXT_ID=0;

void ced_describe_layer(const char *message, int id) {
    if(id >= CED_MAX_LAYER){
        printf("WARNING: ced_describe_layer: Index out of range!\n");
        return;
    }

	CED_TEXT *text = (CED_TEXT*) ced_add(TEXT_ID);
	if(!text){
        printf("WARNING: ced_describe_layer: cant register CED_TEXT");  
        return;
    }

    strncpy(text->text,message,CED_MAX_LAYER_CHAR-1);
    text->text[CED_MAX_LAYER_CHAR-1] = 0;
    text->id=id;
}

static unsigned CONER_ID=0;

void ced_cone_r(float base, float height, double *center, double *rotate, unsigned int layer, float *RGBAcolor) {
    ced_cone_r_ID(base,height,center,rotate,layer, RGBAcolor, 0);
}

void ced_cone_r_ID(float base, float height, double *center, double *rotate, unsigned int layer, float *RGBAcolor, int lcioid) {
	CED_ConeR * cone = (CED_ConeR*) ced_add(CONER_ID);
	if ( ! cone ) return;
	
	cone->base = base;
  	cone->height = height;

 if(layer > 255){ //downward compability
    cone->layer=layer >> CED_LAYER_SHIFT;
 }else{
    cone->layer=layer;
 }
    cone->lcioid = lcioid;
 	
  	const unsigned int dim = 3;
  	const unsigned int channel = 4;
  	unsigned int i, j;

  	for (i = 0; i < dim; i ++ ) {
		cone->center[i] = center[i];
		cone->rotate[i] = rotate[i];
	}
	for (j = 0; j < channel; j ++ ) {
		cone->RGBAcolor[j] = RGBAcolor[j];
	}
}

static unsigned ELLIPSOID_ID=0;

void ced_ellipsoid_r(double *size, double *center, double *rotate, unsigned int layer, int color) {
    ced_ellipsoid_r_ID(size, center, rotate, layer, color, 0);
}

void ced_ellipsoid_r_ID(double *size, double *center, double *rotate, unsigned int layer, int color, int lcioID) {
	
	CED_EllipsoidR * eli = (CED_EllipsoidR*) ced_add(ELLIPSOID_ID);
	if ( ! eli ) return;	
 	
  	const unsigned int dim = 3;
  	unsigned int i;
  	for (i = 0; i < dim; i ++ ) {
		eli->center[i] = center[i];
		eli->rotate[i] = rotate[i];
		eli->size[i] = size[i];
	}
	eli->color = color;

 if(layer > 255){ //downward compability
    eli->layer=layer >> CED_LAYER_SHIFT;
 }else{
    eli->layer=layer;
 }
    eli->lcioid = lcioID;
}

static unsigned CLUELLIPSE_ID=0;

void ced_cluellipse_r(float radius, float height, float *center, double *rotate, unsigned int layer, int color) {
    ced_cluellipse_r_ID(radius, height, center, rotate, layer, color, 0);
}

void ced_cluellipse_r_ID(float radius, float height, float *center, double *rotate, unsigned int layer, int color, int lcioid) {
	
	CED_CluEllipseR * eli = (CED_CluEllipseR*) ced_add(CLUELLIPSE_ID);
	if ( ! eli ) return;	
 	
  	const unsigned int dim = 3;
  	unsigned int i;
  	for (i = 0; i < dim; i ++ ) {
		eli->center[i] = center[i];
		eli->rotate[i] = rotate[i];
	}
	eli->radius = radius;
	eli->height = height;

 if(layer > 255){ //downward compability
    eli->layer=layer >> CED_LAYER_SHIFT;
 }else{
    eli->layer=layer;
 }
	eli->color = color;
    eli->lcioid=lcioid;
}


void ced_register_elements(void){

  //1:
  GEOC_ID		=ced_register_element(sizeof(CED_GeoCylinder),0);
  //2:
  GEOCR_ID	    =ced_register_element(sizeof(CED_GeoCylinderR), 0);
  //3:
  LINE_ID		=ced_register_element(sizeof(CED_Line),0);
  //4:
  HIT_ID		=ced_register_element(sizeof(CED_Hit),0);
  //5:
  GEOB_ID		=ced_register_element(sizeof(CED_GeoBox), 0);
  //6:
  GEOBR_ID	    =ced_register_element(sizeof(CED_GeoBoxR), 0);
  //7:
  GEOBRS_ID	    =ced_register_element(sizeof(CED_GeoBoxR), 0);
  //8:
  CONER_ID	    =ced_register_element(sizeof(CED_ConeR), 0);
  //9:
  ELLIPSOID_ID	=ced_register_element(sizeof(CED_EllipsoidR), 0);
  //10:
  CLUELLIPSE_ID =ced_register_element(sizeof(CED_CluEllipseR), 0);
  //11:
  TEXT_ID       =ced_register_element(sizeof(CED_TEXT),0); // the order of this items is important
  //12:
  LEGEND_ID	    =ced_register_element(sizeof(CED_Legend), 0);
  //13:
  GEOT_ID       =ced_register_element(sizeof(CED_GeoTube),0);
}

