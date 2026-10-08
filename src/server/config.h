/**********************************************************
* config.h, CED server config file                        *
* Headerfile to adapt CED before the build.               *
**********************************************************/

#ifndef __CED_CONFIG
#define __CED_CONFIG



/**********************************************************
* Colors and appearance                                   *
**********************************************************/
//size of the boarder line in filled (new view) of detector components.
#define CED_GEOTUBE_LINE_WIDTH              0.3

//maximal transparency of boarder lines
#define CED_GEOTUBE_LINE_MAX_TRANS          1.0


//Color of xyz axes
#define AXES_COLOR                          0.2,0.2,0.8

//Width of xyz axes
#define AXES_LINE_SIZE                      0.5


//Help frame: Frame boarder line width
#define HELP_FRAME_BOARDER_LINE_SIZE        3.


/**********************************************************
* Graphics                                                *
**********************************************************/

//Camera field of view, in degree
#define CAMERA_FIELD_OF_VIEW                45

//Camera min distance (hint: min and max should be close together)
#define CAMERA_MIN_DISTANCE                 100

//Camera max distance (hint: min and max should be close together)
#define CAMERA_MAX_DISTANCE                 50000.0*mm.sf+50000/mm.sf

//Where the camera stands
#define CAMERA_POSITION                     0,0,2000


#endif

