#ifndef _TRACK_PAINT_H
#define _TRACK_PAINT_H

#include "../common.h"

typedef void (*TRACK_PAINT_FUNCTION)(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement);
typedef TRACK_PAINT_FUNCTION (*TRACK_PAINT_FUNCTION_GETTER)(int trackType, int direction);

TRACK_PAINT_FUNCTION get_track_paint_function_maze(int trackType, int direction);
TRACK_PAINT_FUNCTION get_track_paint_function_topspin(int trackType, int direction);
TRACK_PAINT_FUNCTION get_track_paint_function_shop(int trackType, int direction);
TRACK_PAINT_FUNCTION get_track_paint_function_facility(int trackType, int direction);
TRACK_PAINT_FUNCTION get_track_paint_function_identify();
TRACK_PAINT_FUNCTION get_track_paint_function_identify_1x4();
TRACK_PAINT_FUNCTION get_track_paint_function_identify_1x5();
TRACK_PAINT_FUNCTION get_track_paint_function_identify_2x2();
TRACK_PAINT_FUNCTION get_track_paint_function_identify_3x3();
TRACK_PAINT_FUNCTION get_track_paint_function_identify_4x4();

#endif
