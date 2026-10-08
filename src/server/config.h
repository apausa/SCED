/**********************************************************
* config.h, CED server config file                        *
* Headerfile to adapt CED before the build.               *
**********************************************************/

#ifndef __CED_CONFIG
#define __CED_CONFIG



/**********************************************************
* Handling                                                *
**********************************************************/
//enable zoom function by right click and pull
#define ZOOM_RIGHT_CLICK                   0

//time when 2 clicks should be a double click, in 1/1000000 secounds
#define DOUBLE_CLICK_TIME                  300000

//data layer keys
#define DATALAYER_SHORTKEY_00       '0'
#define DATALAYER_SHORTKEY_01       '1'
#define DATALAYER_SHORTKEY_02       '2'
#define DATALAYER_SHORTKEY_03       '3'
#define DATALAYER_SHORTKEY_04       '4'
#define DATALAYER_SHORTKEY_05       '5'
#define DATALAYER_SHORTKEY_06       '6'
#define DATALAYER_SHORTKEY_07       '7'
#define DATALAYER_SHORTKEY_08       '8'
#define DATALAYER_SHORTKEY_09       '9'
#define DATALAYER_SHORTKEY_10       ')'
#define DATALAYER_SHORTKEY_11       '!'
#define DATALAYER_SHORTKEY_12       '@'
#define DATALAYER_SHORTKEY_13       '#'
#define DATALAYER_SHORTKEY_14       '$'
#define DATALAYER_SHORTKEY_15       '%'
#define DATALAYER_SHORTKEY_16       '^'
#define DATALAYER_SHORTKEY_17       '&'
#define DATALAYER_SHORTKEY_18       '*'
#define DATALAYER_SHORTKEY_19       '('
#define DATALAYER_SHORTKEY_20       't'
#define DATALAYER_SHORTKEY_21       'y'
#define DATALAYER_SHORTKEY_22       'u'
#define DATALAYER_SHORTKEY_23       'i'
#define DATALAYER_SHORTKEY_24       'o'

//detector layer keys
#define DETECTORLAYER_SHORTKEY_00   'j'
#define DETECTORLAYER_SHORTKEY_01   'k'
#define DETECTORLAYER_SHORTKEY_02   'l'
#define DETECTORLAYER_SHORTKEY_03   ';'
#define DETECTORLAYER_SHORTKEY_04   '\''
#define DETECTORLAYER_SHORTKEY_05   'p'
#define DETECTORLAYER_SHORTKEY_06   '['
#define DETECTORLAYER_SHORTKEY_07   ']'
#define DETECTORLAYER_SHORTKEY_08   '\\'
#define DETECTORLAYER_SHORTKEY_09   'T'
#define DETECTORLAYER_SHORTKEY_10   'Y'
#define DETECTORLAYER_SHORTKEY_11   'U'
#define DETECTORLAYER_SHORTKEY_12   'I'
#define DETECTORLAYER_SHORTKEY_13   'O'
#define DETECTORLAYER_SHORTKEY_14   'P'
#define DETECTORLAYER_SHORTKEY_15   '{'
#define DETECTORLAYER_SHORTKEY_16   '}'
#define DETECTORLAYER_SHORTKEY_17   '|'
#define DETECTORLAYER_SHORTKEY_18   'a'
#define DETECTORLAYER_SHORTKEY_19   'e'
#define DETECTORLAYER_SHORTKEY_20   'A'
#define DETECTORLAYER_SHORTKEY_21   'B'
#define DETECTORLAYER_SHORTKEY_22   'D'
#define DETECTORLAYER_SHORTKEY_23   'E'
#define DETECTORLAYER_SHORTKEY_24   'G'
#define DETECTORLAYER_SHORTKEY_25   'H'
#define DETECTORLAYER_SHORTKEY_26   'J'
#define DETECTORLAYER_SHORTKEY_27   'K'
#define DETECTORLAYER_SHORTKEY_28   'L'
#define DETECTORLAYER_SHORTKEY_29   'N'
#define DETECTORLAYER_SHORTKEY_30   'Q'
#define DETECTORLAYER_SHORTKEY_31   'W'
#define DETECTORLAYER_SHORTKEY_32   'X'
#define DETECTORLAYER_SHORTKEY_33   'd'
#define DETECTORLAYER_SHORTKEY_34   'g'
#define DETECTORLAYER_SHORTKEY_35   'n'
#define DETECTORLAYER_SHORTKEY_36   'q'
#define DETECTORLAYER_SHORTKEY_37   'w'
#define DETECTORLAYER_SHORTKEY_38   ':'
#define DETECTORLAYER_SHORTKEY_39   '.'



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


//Help frame: Frame fill color, and transp
#define HELP_FRAME_FILL_COLOR               0.5,1,1,0.8

//Help frame: Frame boarder color and transp
#define HELP_FRAME_BOARDER_COLOR            0.1,0.8,1.0,0.8

//Help frame: Frame boarder line width
#define HELP_FRAME_BOARDER_LINE_SIZE        3.

//Help frame: Text color, and transp
#define HELP_FRAME_TEXT_COLOR               0.0,0.0,0.0


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

