/* "C" event display.
 * Data structures shared by the client and the server.
 *
 * Alexey Zhelezov, DESY/ITEP, 2005 
 */

#ifndef __CED_COMMON_H
#define __CED_COMMON_H

//important:
//          - sum of all layers must be smaler than max_layer!
//          - number_popup_layer must be smaler than number_data_layer

//#define CED_MAX_LAYER       100
//#define NUMBER_POPUP_LAYER      20
//#define NUMBER_DATA_LAYER       25
//#define NUMBER_DETECTOR_LAYER   20
//
//
//#define CED_MAX_LAYER_CHAR 400


/**********************************************************
* Layers                                                  *
**********************************************************/
//number of total number of layers
#define CED_MAX_LAYER                       120

//number of layers shown in popup menu
#define NUMBER_POPUP_LAYER                  20

//number of layers reserved for data
#define NUMBER_DATA_LAYER                   25

//number of layers reserved for detector components
#define NUMBER_DETECTOR_LAYER               40

//layer description text: maximal number of chars for one entry
#define CED_MAX_LAYER_CHAR                  400

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Register the element types. Both the client and the server
 * implement this function, and the order must be the same.
 */
void ced_register_elements(void);

typedef struct {
  float x;
  float y;
  float z;
} CED_Point;

//class CED_Point{
//  public:
//  CED_Point(float _x, float _y, float _z){
//       x=_x;
//       y=_y;
//       z=_z;
//   }
//
//  CED_Point(void){
//  }
//  float x;
//  float y;
//  float z;
//}; 


/*
 * Hit element
 */

typedef enum {
    CED_HIT_POINT=0,
    CED_HIT_CROSS,
    CED_HIT_STAR,
    CED_HIT_BOX,
    CED_HIT_VXD
} CED_HIT_TYPE;

typedef struct {
  CED_Point p;
  float time; // allows event animation
  unsigned type;  // point, star, etc
  unsigned layer; //layer
  unsigned color; // in ARGB form (so, 0xff0000 is RED)
  unsigned size;  // size of point/size of cross
  unsigned lcioID; // unique id of LICO object
} CED_Hit;

/*
 * Line element
 */

typedef struct {
  CED_Point p0;
  CED_Point p1;
  unsigned type;  // not yet defined...
  unsigned width; // not yet defined...
  unsigned color; // in ARGB form (so, 0xff0000 is RED)
  unsigned lcioID; // unique id of LICO object

} CED_Line;

/*
 * GeoCylinder
 */
typedef struct {
  float d;       // radius
  unsigned  sides;   // poligon order
  float rotate;  // angle degree
  float z;       // 1/2 length
  float shift;   // in z
  unsigned color;
} CED_GeoCylinder;

/*
 * GeoTube
 */
typedef struct {
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
}  CED_GeoTube;

/*
 * GeoCylinder rotatable
 * @author: S.Daraszewicz (UoE)
 * @date: 01.09.09
 */
typedef struct {
  float d;       	// radius
  unsigned sides; 	// poligon order
  float center[3];  // cylinder centre z,y,z
  float rotate[3];  // rotation angles wrt x,y,z axis
  float z;       	// length
  unsigned color;	// colour
  unsigned layer; 	// layer the Cylinder to be displayed onto
} CED_GeoCylinderR;

  /** GeoBox structure
   */
  typedef struct {
    /** The three box sizes in mm */
    double sizes[3];
    /** position of the center of the box*/
    double center[3];
    /** box color */
    unsigned int color;
  } CED_GeoBox;

  typedef struct {
    /** The three box sizes in mm */
    double sizes[3];
    /** position of the center of the box*/
    double center[3];
    /** box color */
    unsigned int color;
    /** rotation angle in degrees */
    double rotate[3];
    /** layer for toggling display */
    unsigned int layer;
  } CED_GeoBoxR;

  typedef struct{
    char text[400];
    int id;
  } CED_TEXT; 

/*
 * Energy spectrum colour map legend.
 * @author: S.Daraszewicz (UoE)
 * @date: 01.09.09
 */
  typedef struct {  
  	/** min energy on the legend */	
  	float ene_max;
  	/** max energy on the legend */
  	float ene_min;
  	/** number of ticks on the legend */
  	unsigned int ticks;
  	/** spectrum colour steps */
  	unsigned int color_steps; 
  	/** spectrum colour matrix */
  	unsigned int rgb_matrix[512][3]; //FIX ME: 512 size not changed with color_steps
  	/** LOG or LIN */
  	char scale;
  } CED_Legend;

  typedef struct {  
  	/** position of the centre of the base */	
  	double center[3];
  	/** rotation matrix */
  	double rotate[3];
    /** layer for toggling display */
    unsigned int layer;
    /** base radius */
    float base;
    /** height */
    float height;
    /** RGBA color */
    float RGBAcolor[4];
    unsigned lcioid; //hauke
  } CED_ConeR;

  typedef struct {  
  	/** position of the centre of the base */	
  	double center[3];
  	/** rotation matrix */
  	double rotate[3];
    /** layer for toggling display */
    unsigned int layer;
    /** xyz size */
	double size[3];
    /** RGBA color */
   	int color;
    unsigned lcioid; //hauke
  } CED_EllipsoidR;

  typedef struct {  
  	/** position of the centre of the base */	
  	double center[3];
  	/** rotation matrix */
  	double rotate[3];
    /** layer for toggling display */
    unsigned int layer;
    /** base radius */
	float radius;
	/** half height */
	float height;
    /** RGBA color */
    int color;
    unsigned lcioid; //hauke
  } CED_CluEllipseR;

#ifdef __cplusplus
 }
#endif
	

#endif /* __CED_COMMON_H */

