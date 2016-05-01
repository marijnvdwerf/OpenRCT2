#pragma region Copyright (c) 2014-2016 OpenRCT2 Developers
/*****************************************************************************
* OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
*
* OpenRCT2 is the work of many authors, a full list can be found in contributors.md
* For more information, visit https://github.com/OpenRCT2/OpenRCT2
*
* OpenRCT2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* A full copy of the GNU General Public License can be found in licence.txt
*****************************************************************************/
#pragma endregion

#include "../paint.h"
#include "../../interface/viewport.h"
#include "../../addresses.h"
#include "map_element.h"
#include "../../drawing/drawing.h"
#include "../../ride/ride_data.h"
#include "../../ride/track_data.h"
#include "../../ride/track_paint.h"
#include "../../config.h"
#include "../../world/sprite.h"
#include "../../world/banner.h"
#include "../../world/entrance.h"
#include "../../world/footpath.h"
#include "../../world/scenery.h"
#include "../../sprites.h"
#include "../../localisation/localisation.h"
#include "../../game.h"
#include "../supports.h"

static void viewport_blank_tiles_paint_setup(int x, int y);
static void sub_68B3FB(int x, int y);
void viewport_track_paint_setup(uint8 direction, int height, rct_map_element *mapElement);

/**
 *
 *  rct2: 0x0068B35F
 */
void map_element_paint_setup(int x, int y)
{
	rct_drawpixelinfo *dpi = RCT2_GLOBAL(0x0140E9A8, rct_drawpixelinfo*);
	if (
		x < gMapSizeUnits &&
		y < gMapSizeUnits &&
		x >= 32 &&
		y >= 32
	) {
		RCT2_GLOBAL(0x0141E9B4, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9B8, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9BC, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9C0, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9C4, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9C8, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9CC, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9D0, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9D4, uint32) = 0xFFFF;
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = -1;
		RCT2_GLOBAL(0x0141E9DA, sint16) = 0;
		RCT2_GLOBAL(0x0141E9DC, uint32) = 0xFFFF;

		sub_68B3FB(x, y);
	} else {
		viewport_blank_tiles_paint_setup(x, y);
	}
}

/**
 *
 *  rct2: 0x0068B2B7
 */
void sub_68B2B7(int x, int y)
{
	if (
		x < gMapSizeUnits &&
		y < gMapSizeUnits &&
		x >= 32 &&
		y >= 32
	) {
		RCT2_GLOBAL(0x0141E9B4, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9B8, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9BC, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9C0, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9C4, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9C8, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9CC, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9D0, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9D4, uint32) = 0xFFFF;
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = -1;
		RCT2_GLOBAL(0x0141E9DA, uint16) = 0;
		RCT2_GLOBAL(0x0141E9DC, uint32) = 0xFFFF;
		RCT2_GLOBAL(0x0141E9DB, uint8) |= 2;

		sub_68B3FB(x, y);
	} else {
		viewport_blank_tiles_paint_setup(x, y);
	}
}

/**
 *
 *  rct2: 0x0068B60E
 */
static void viewport_blank_tiles_paint_setup(int x, int y)
{
	rct_drawpixelinfo *dpi = RCT2_GLOBAL(0x0140E9A8, rct_drawpixelinfo*);

	int dx;
	switch (get_current_rotation()) {
	case 0:
		dx = x + y;
		break;
	case 1:
		x += 32;
		dx = y - x;
		break;
	case 2:
		x += 32;
		y += 32;
		dx = -(x + y);
		break;
	case 3:
		y += 32;
		dx = x - y;
		break;
	}
	dx /= 2;
	dx -= 16;
	int bx = dx + 32;
	if (bx <= dpi->y) return;
	dx -= 20;
	dx -= dpi->height;
	if (dx >= dpi->y) return;
	RCT2_GLOBAL(0x9DE568, sint16) = x;
	RCT2_GLOBAL(0x9DE56C, sint16) = y;
	RCT2_GLOBAL(RCT2_ADDRESS_PAINT_SETUP_CURRENT_TYPE, uint8_t) = VIEWPORT_INTERACTION_ITEM_NONE;
	RCT2_CALLPROC_X(
		(int)RCT2_ADDRESS(0x98196C, uint32_t*)[get_current_rotation()],
		0xFF00,
		3123,
		y & 0xFF00,
		16,
		32,
		32,
		get_current_rotation()
	);
}

/**
 *
 *  rct2: 0x0068B3FB
 */
static void sub_68B3FB(int x, int y)
{
	rct_drawpixelinfo *dpi = RCT2_GLOBAL(0x0140E9A8, rct_drawpixelinfo*);

	RCT2_GLOBAL(0x141F56A, uint16_t) = 0;
	RCT2_GLOBAL(0x9E3138, uint8_t) = 0xFF;
	RCT2_GLOBAL(0x9E30B6, uint8_t) = 0xFF;
	RCT2_GLOBAL(0x9E323C, uint8_t) = 0xFF;
	RCT2_GLOBAL(0x9DE56A, uint16_t) = x;
	RCT2_GLOBAL(0x9DE56E, uint16_t) = y;
	RCT2_GLOBAL(0x9DE574, uint16_t) = x;
	RCT2_GLOBAL(0x9DE576, uint16_t) = y;

	rct_map_element* map_element = map_get_first_element_at(x >> 5, y >> 5);
	uint8 rotation = get_current_rotation();

	int dx = 0;
	switch (rotation) {
	case 0:
		dx = x + y;
		break;
	case 1:
		x += 32;
		dx = y - x;
		break;
	case 2:
		x += 32;
		y += 32;
		dx = -(x + y);
		break;
	case 3:
		y += 32;
		dx = x - y;
		break;
	}
	dx >>= 1;
	// Display little yellow arrow when building footpaths?
	if ((RCT2_GLOBAL(RCT2_ADDRESS_MAP_SELECTION_FLAGS, uint16) & 4) &&
		RCT2_GLOBAL(0x9DE56A, uint16) == RCT2_GLOBAL(RCT2_ADDRESS_MAP_ARROW_X, uint16) &&
		RCT2_GLOBAL(0x9DE56E, uint16) == RCT2_GLOBAL(RCT2_ADDRESS_MAP_ARROW_Y, uint16)){
		uint8 arrowRotation =
			(rotation
			+ (RCT2_GLOBAL(RCT2_ADDRESS_MAP_ARROW_DIRECTION, uint8) & 3)) & 3;

		uint32 imageId =
			arrowRotation +
			(RCT2_GLOBAL(RCT2_ADDRESS_MAP_ARROW_DIRECTION, uint8) & 0xFC) +
			0x20900C27;
		int arrowZ = RCT2_GLOBAL(RCT2_ADDRESS_MAP_ARROW_Z, uint16);

		RCT2_GLOBAL(0x9DE568, sint16) = x;
		RCT2_GLOBAL(0x9DE56C, sint16) = y;
		RCT2_GLOBAL(RCT2_ADDRESS_PAINT_SETUP_CURRENT_TYPE, uint8) = VIEWPORT_INTERACTION_ITEM_NONE;

		sub_98197C(imageId, 0, 0, 32, 32, 0xFF, arrowZ, 0, 0, arrowZ + 18, rotation);
	}
	int bx = dx + 52;

	if (bx <= dpi->y)
		return;

	const rct_map_element* element = map_element;//push map_element

	sint16 max_height = 0;
	do{
		max_height = max(max_height, element->clearance_height);
	} while (!map_element_is_last_for_tile(element++));

	element--;

	if (map_element_get_type(element) == MAP_ELEMENT_TYPE_SURFACE &&
		(element->properties.surface.terrain & MAP_ELEMENT_WATER_HEIGHT_MASK) != 0){
		max_height = (element->properties.surface.terrain & MAP_ELEMENT_WATER_HEIGHT_MASK) << 1;
	}

	max_height *= 8;

	dx -= max_height + 32;

	element = map_element;//pop map_element
	dx -= dpi->height;
	if (dx >= dpi->y)
		return;

	RCT2_GLOBAL(0x9DE568, sint16) = x;
	RCT2_GLOBAL(0x9DE56C, sint16) = y;
	RCT2_GLOBAL(0x9DE57C, uint16) = 0;
	do {
		int direction = (map_element->type + rotation) & MAP_ELEMENT_DIRECTION_MASK;
		int height = map_element->base_height * 8;

		uint32_t dword_9DE574 = RCT2_GLOBAL(0x9DE574, uint32_t);
		RCT2_GLOBAL(0x9DE578, rct_map_element*) = map_element;
		//setup the painting of for example: the underground, signs, rides, scenery, etc.
		switch (map_element_get_type(map_element))
		{
		case MAP_ELEMENT_TYPE_SURFACE:
			RCT2_CALLPROC_X(0x66062C, 0, 0, direction, height, (int)map_element, 0, 0);
			break;
		case MAP_ELEMENT_TYPE_PATH:
			RCT2_CALLPROC_X(0x6A3590, 0, 0, direction, height, (int)map_element, 0, 0);
			break;
		case MAP_ELEMENT_TYPE_TRACK:
			viewport_track_paint_setup(direction, height, map_element);
			break;
		case MAP_ELEMENT_TYPE_SCENERY:
			RCT2_CALLPROC_X(0x6DFF47, 0, 0, direction, height, (int)map_element, 0, 0);
			break;
		case MAP_ELEMENT_TYPE_ENTRANCE:
			viewport_entrance_paint_setup(direction, height, map_element);
			break;
		case MAP_ELEMENT_TYPE_FENCE:
			RCT2_CALLPROC_X(0x6E44B0, 0, 0, direction, height, (int)map_element, 0, 0);
			break;
		case MAP_ELEMENT_TYPE_SCENERY_MULTIPLE:
			RCT2_CALLPROC_X(0x6B7F0C, 0, 0, direction, height, (int)map_element, 0, 0);
			break;
		case MAP_ELEMENT_TYPE_BANNER:
			viewport_banner_paint_setup(direction, height, map_element);
			break;
		// A corrupt element inserted by OpenRCT2 itself, which skips the drawing of the next element only.
		case MAP_ELEMENT_TYPE_CORRUPT:
			if (map_element_is_last_for_tile(map_element))
				return;
			map_element++;
			break;
		default:
			// An undefined map element is most likely a corrupt element inserted by 8 cars' MOM feature to skip drawing of all elements after it.
			return;
		}
		RCT2_GLOBAL(0x9DE574, uint32_t) = dword_9DE574;
	} while (!map_element_is_last_for_tile(map_element++));
}

/**
 *
 *  rct2: 0x006C4794
 */
void viewport_track_paint_setup(uint8 direction, int height, rct_map_element *mapElement)
{
	rct_drawpixelinfo *dpi = RCT2_GLOBAL(0x0140E9A8, rct_drawpixelinfo*);
	rct_ride *ride;
	int rideIndex, trackType, trackColourScheme, trackSequence;

	rideIndex = mapElement->properties.track.ride_index;
	ride = get_ride(rideIndex);
	if (ride->type == RIDE_TYPE_NULL) {
		log_error("Attempted to paint invalid ride: %d", rideIndex);
		return;
	}

	// HACK Set entrance style to plain if none to stop glitch until entrance track piece is implemented
	bool isEntranceStyleNone = false;
	if (ride->entrance_style == RIDE_ENTRANCE_STYLE_NONE) {
		isEntranceStyleNone = true;
		ride->entrance_style = RIDE_ENTRANCE_STYLE_PLAIN;
	}

	if (!(RCT2_GLOBAL(0x009DEA6F, uint8) & 1) || rideIndex == RCT2_GLOBAL(0x00F64DE8, uint8)) {
		trackType = mapElement->properties.track.type;
		trackSequence = mapElement->properties.track.sequence & 0x0F;
		trackColourScheme = mapElement->properties.track.colour & 3;

		if ((gCurrentViewportFlags & VIEWPORT_FLAG_TRACK_HEIGHTS) && dpi->zoom_level == 0) {
			RCT2_GLOBAL(RCT2_ADDRESS_PAINT_SETUP_CURRENT_TYPE, uint8) = 0;
			if (RCT2_ADDRESS(0x00999694, uint32)[trackType] & (1 << trackSequence)) {
				uint16 ax = RideData5[ride->type].z_offset;
				uint32 ebx = 0x20381689 + (height + 8) / 16;
				ebx += RCT2_GLOBAL(RCT2_ADDRESS_CONFIG_HEIGHT_MARKERS, uint16);
				ebx -= RCT2_GLOBAL(0x01359208, uint16);
				sub_98197C(ebx, 16, 16, 1, 1, 0, height + ax + 3, 1000, 1000, 2047, get_current_rotation());
			}
		}

		RCT2_GLOBAL(RCT2_ADDRESS_PAINT_SETUP_CURRENT_TYPE, uint8) = 3;
		RCT2_GLOBAL(0x00F44198, uint32) = (ride->track_colour_main[trackColourScheme] << 19) | (ride->track_colour_additional[trackColourScheme] << 24) | 0xA0000000;
		RCT2_GLOBAL(0x00F441A0, uint32) = 0x20000000;
		RCT2_GLOBAL(0x00F441A4, uint32) = 0x20C00000;
		RCT2_GLOBAL(0x00F4419C, uint32) = (ride->track_colour_supports[trackColourScheme] << 19) | 0x20000000;
		if (mapElement->type & 0x40) {
			RCT2_GLOBAL(0x00F44198, uint32) = 0x21600000;
			RCT2_GLOBAL(0x00F4419C, uint32) = 0x21600000;
			RCT2_GLOBAL(0x00F441A0, uint32) = 0x21600000;
			RCT2_GLOBAL(0x00F441A4, uint32) = 0x21600000;
		}
		if (mapElement->flags & MAP_ELEMENT_FLAG_GHOST) {
			uint32 ghost_id = RCT2_ADDRESS(0x00993CC4, uint32)[gConfigGeneral.construction_marker_colour];
			RCT2_GLOBAL(RCT2_ADDRESS_PAINT_SETUP_CURRENT_TYPE, uint8) = 0;
			RCT2_GLOBAL(0x00F44198, uint32) = ghost_id;
			RCT2_GLOBAL(0x00F4419C, uint32) = ghost_id;
			RCT2_GLOBAL(0x00F441A0, uint32) = ghost_id;
			RCT2_GLOBAL(0x00F441A4, uint32) = ghost_id;
		}

		int rideType = ride->type;
		if (rideType == RIDE_TYPE_JUNIOR_ROLLER_COASTER) {
			switch (trackType) {
			case TRACK_ELEM_60_DEG_UP:
			case TRACK_ELEM_25_DEG_UP_TO_60_DEG_UP:
			case TRACK_ELEM_60_DEG_UP_TO_25_DEG_UP:
			case TRACK_ELEM_60_DEG_DOWN:
			case TRACK_ELEM_25_DEG_DOWN_TO_60_DEG_DOWN:
			case TRACK_ELEM_60_DEG_DOWN_TO_25_DEG_DOWN:
				rideType = RIDE_TYPE_WATER_COASTER;
				break;

			case TRACK_ELEM_FLAT_TO_60_DEG_UP:
			case TRACK_ELEM_60_DEG_UP_TO_FLAT:
			case TRACK_ELEM_FLAT_TO_60_DEG_DOWN:
			case TRACK_ELEM_60_DEG_DOWN_TO_FLAT:
				return;
			}
		}

		TRACK_PAINT_FUNCTION **trackTypeList = (TRACK_PAINT_FUNCTION**)RideTypeTrackPaintFunctionsOld[rideType];
		if (trackTypeList == NULL) {
			TRACK_PAINT_FUNCTION_GETTER paintFunctionGetter = RideTypeTrackPaintFunctions[rideType];
			TRACK_PAINT_FUNCTION paintFunction = paintFunctionGetter(trackType, direction);
			if (paintFunction != NULL) {
				paintFunction(rideIndex, trackSequence, direction, height, mapElement);
			}
		}
		else {
			uint32 *trackDirectionList = (uint32*)trackTypeList[trackType];

			// Have to call from this point as it pushes esi and expects callee to pop it
			RCT2_CALLPROC_X(
				0x006C4934,
				rideType,
				(int)trackDirectionList,
				direction,
				height,
				(int)mapElement,
				rideIndex * sizeof(rct_ride),
				trackSequence
				);
		}
	}

	if (isEntranceStyleNone) {
		ride->entrance_style = RIDE_ENTRANCE_STYLE_NONE;
	}
}
