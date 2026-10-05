/* "C" event display.
 * Enduser accessable API (client side).
 *
 * Alexey Zhelezov, DESY/ITEP, 2005 
 */

#ifndef __CED_CLI_H
#define __CED_CLI_H

#include <ced_common.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
 * This is the first function to call (before any other).
 *
 *  host - host with CED (must be "localhost")
 *  port - server port number (let say 7285 :)
 *
 * NOTE: ced_register_elements() must be called
 *       separately.
 */
void ced_client_init(const char *host,unsigned short port);

/*
 * Cancel current event output. So, all elements
 * queued will be discarded.
 *
 * Good to call at the begining of every event processing.
 */
void ced_new_event(void);


/*
 * This function really attempt to display event in CED.
 * When CED is not available, this function discard
 * current event information.
 *
 * NOTE: between ced_new_event() and ced_draw_event()
 *       must be some element creation calls.
 */
void ced_draw_event(void);

/*
 * This function really attempt to display event in CED.
 * Unlike ced_draw_event() does not reset the event.
 *
 * NOTE: between ced_new_event() and ced_draw_event()
 *       must be some element creation calls.
 */
void ced_send_event(void);

int ced_selected_id(void);

//hauke
int ced_selected_id_noblock(void);


/*********************************************
 *
 * The following is elements API.
 *
 *********************************************/

typedef enum {
  CED_TYPE_SHIFT=0x0,
  CED_LAYER_SHIFT=0x8
} CED_TYPE_BITS;

/*
typedef enum {
  CED_TYPE_SHIFT=0x0,
  CED_LAYER_SHIFT=0x0
} CED_TYPE_BITS;
*/


/*
 * Hit element
 */

void ced_hit(float x,float y,float z,unsigned type,unsigned size,unsigned color);

//to give a bit of downward compatibility
void ced_hit_ID_old(float x,float y,float z,unsigned type, unsigned size,unsigned color, unsigned lcioID);


void ced_hit_ID(float x,float y,float z,unsigned type,unsigned layer, unsigned size,unsigned color, unsigned lcioID);

void ced_hit_ID_animate(float x,float y,float z,float t, unsigned type,unsigned layer, unsigned size,unsigned color, unsigned lcioID);

/*
 * Line element
 */

void ced_line(float x0,float y0,float z0,
	      float x1,float y1,float z1,
	      unsigned type,unsigned width,unsigned color);
void ced_line_ID(float x0,float y0,float z0,
	      float x1,float y1,float z1,
	      unsigned type,unsigned width,unsigned color, unsigned lcioID);

/** Same as CED_GeoTube but here as C++ struct with contstructor. This is allows
 *  to dynamically allocate the detector structure (in an std::vector using the constructor) 
 *  without knowing the exact number of  detector elements a priori (such as the #layers in the FTD).
 *  This wasn't poassible with the static allocation using the C type struct.
 *  ( Used in MArlinCED::drawGearDetector ).
 */
struct CEDGeoTube{
  CEDGeoTube(double  r_out,
	     double  r_in,
	     int edges_out,
	     int edges_in,
	     double  rotate_out,
	     double  rotate_in,
	     double  zlength,
	     double  zshift,
	     int col,
	     int layer,
	     bool classic_i,
	     bool classic_o ) :
    r_o(r_out),
    r_i(r_in),
    edges_o(edges_out),
    edges_i (edges_in),
    rotate_o (rotate_out),
    rotate_i (rotate_in),
    z( zlength),
    shift (zshift),
    color (col),
    type(layer),
    classic_inner(classic_i),
    classic_outer(classic_o) {} 
  float r_o;            // outer radius
  float r_i;            // inner radius
  unsigned edges_o;     // edges outer
  unsigned edges_i;     // edges inner
  float rotate_o;       // angle degree, rotate outer cylinder
  float rotate_i;       //rotate inner cylinder
  float z;              // 1/2 length
  float shift;          // shift in z
  unsigned color;       // color
  unsigned type;        //describes the layer where this element lies
  bool classic_inner;   //draw the outer detector lines in classic view?
  bool classic_outer;   //draw the inner detector lines in classic view?
} ;

void ced_geotubes(unsigned n,CED_GeoTube *all);


void ced_geocylinder(float d,unsigned sides,float rotate,float z,float shift,
		     unsigned color);

void ced_geocylinders(unsigned n,CED_GeoCylinder *all);

void ced_geocylinder_r(float d, double z, double * center, double * rotate, unsigned sides, 
		     unsigned int color, int layer);


  /** Send/Draw a box at position center (x,y,z in mm) with lengths along the 
   * axes specified in sizes.
   * 
   * @author A.Bulgheroni, INFN
   */
  void ced_geobox(double * sizes, double * center, unsigned int color );
  void ced_geobox_ID(double *size, double *position, unsigned int layer, unsigned int color, unsigned int lcio_id);
  void ced_geobox_r_ID(double *size, double *position, double *rotate, unsigned int layer, unsigned int color, unsigned int lcio_id);
  void rotate3d(double *vektor, double *rotate);


   /* 
   * @author A.Bulgheroni, INFN
   */
  void ced_geoboxes( unsigned int nBox, CED_GeoBox * allBoxes);

void ced_geobox_r(double * sizes, double * center, double * rotate, unsigned int color, unsigned int layer);
void ced_geobox_r_solid(double * sizes, double * center, double * rotate, unsigned int color, unsigned int layer);


void ced_describe_layer(const char *, int); //, int, int);

  typedef struct{
    char str[400];
    unsigned int id;
  } LAYER_TEXT; 

void ced_layer_text(char *, int);
//end hauke

void ced_legend(float ene_min, float ene_max, unsigned int color_steps, unsigned int ** rgb_matrix, unsigned int ticks, char scale);

void ced_cone_r(float base, float height, double *center, double *rotate, unsigned int layer, float *RGBAcolor);
void ced_cone_r_ID(float base, float height, double *center, double *rotate, unsigned int layer, float *RGBAcolor, int lcioid); //hauke

void ced_ellipsoid_r(double *size, double *center, double *rotate, unsigned int layer, int color);
void ced_ellipsoid_r_ID(double *size, double *center, double *rotate, unsigned int layer, int color, int lcioid); //hauke

void ced_cluellipse_r(float radius, float height, float *center, double *rotate, unsigned int layer, int color);
void ced_cluellipse_r_ID(float radius, float height, float *center, double *rotate, unsigned int layer, int color, int lcioid); //hauke


#ifdef __cplusplus
 }
#endif

#endif /* __CED_CLI_H */
