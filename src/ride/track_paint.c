/*****************************************************************************
* Copyright (c) 2014 Ted John
* OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
*
* This file is part of OpenRCT2.
*
* OpenRCT2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "../addresses.h"
#include "../config.h"
#include "../drawing/drawing.h"
#include "../drawing/supports.h"
#include "../interface/viewport.h"
#include "../interface/window.h"
#include "../localisation/localisation.h"
#include "../sprites.h"
#include "../world/map.h"
#include "../world/sprite.h"
#include "ride_data.h"
#include "track_data.h"
#include "track_paint.h"

/**
 *
 *  rct2: 0x0076687C
 */
static void top_spin_paint_tile_0(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	wooden_a_supports_paint_setup(direction & 1, 0, height, image_id, NULL);

	image_id = 22137 | RCT2_GLOBAL(0x00F44198, uint32);
	sub_98197C(image_id, 0, 0, 32, 32, 1, height, 0, 0, height, get_current_rotation());

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D4, uint16) = 0xFFFF;

	height += 112;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/* rct2: 0x007667AE */
static rct_xy16 loc_7667AE[] = {
	{ .x = 0,  .y = -1 },
	{ .x = 1,  .y = 0 },
	{ .x = 0,  .y = 1},
	{ .x = -1, .y = 0 },
};

/* rct2: 0x007667AC */
static rct_xy16 loc_7667AC[] = {
	{ .x = -1, .y = 0 },
	{ .x = 0,  .y = -1 },
	{ .x = 1,  .y = 0 },
	{ .x = 0,  .y = 1 },
};

/**
 *
 *  rct2: 0x0142811C
 * Can be calculated as Rounddown(34*sin(x)+0.5)
 * where x is in 7.5 deg segments.
 */
static sint8 TopSpinSeatPositionOffset[] = {
	  0,   4,   9,  13,  17,  21,  24,  27,  29,  31,  33,  34,  34,  34,  33,  31,
	 29,  27,  24,  21,  17,  13,   9,   4,   0,  -3,  -8, -12, -16, -20, -23, -26,
	-28, -30, -32, -33, -33, -33, -32, -30, -28, -26, -23, -20, -16, -12,  -8,  -3,
	  0
};

/**
 *
 *  rct2: 0x0076750D
 */
static void top_spin_paint_vehicle(sint8 al, sint8 cl, uint8 rideIndex, uint8 direction, int height, rct_map_element* mapElement) {
	uint16 boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ;
	// As we will be drawing a vehicle we need to backup the mapElement that
	// is assigned to the drawings.
	rct_map_element* curMapElement = RCT2_GLOBAL(0x009DE578, rct_map_element*);

	height += 3;

	rct_ride* ride = get_ride(rideIndex);
	rct_ride_entry* rideEntry = get_ride_entry(ride->subtype);
	rct_vehicle* vehicle = NULL;

	uint8 seatRotation = 0;
	sint8 armRotation = 0;

	if (ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK &&
		ride->vehicles[0] != SPRITE_INDEX_NULL) {
		vehicle = GET_VEHICLE(ride->vehicles[0]);

		RCT2_GLOBAL(RCT2_ADDRESS_PAINT_SETUP_CURRENT_TYPE, uint8) = VIEWPORT_INTERACTION_ITEM_SPRITE;
		RCT2_GLOBAL(0x009DE578, rct_vehicle*) = vehicle;

		armRotation = vehicle->vehicle_sprite_type;
		seatRotation = vehicle->bank_rotation;
	}

	boundBoxOffsetX = al + 16;
	boundBoxOffsetY = cl + 16;
	boundBoxOffsetZ = height;

	//di
	uint8 lengthX = 24;
	//si
	uint8 lengthY = 24;

	uint32 image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	if (image_id == 0x20000000) {
		image_id =
			0xA0000000 |
			(ride->track_colour_main[0] << 19) |
			(ride->track_colour_supports[0] << 24);
	}

	image_id += (direction & 1) << 1;
	image_id += rideEntry->vehicles[0].base_image_id;
	// Left back bottom support
	image_id += 572;
	sub_98197C(image_id, al, cl, lengthX, lengthY, 90, height, boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ, get_current_rotation());

	image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	if (image_id == 0x20000000) {
		image_id =
			0xA0000000 |
			(ride->track_colour_main[0] << 19) |
			(ride->track_colour_additional[0] << 24);
	}

	sint32 var_1F = armRotation;
	if (direction & 2) {
		var_1F = -var_1F;
		if (var_1F != 0)
			var_1F += 48;
	}
	image_id += var_1F;
	image_id += (direction & 1) * 48;
	image_id += rideEntry->vehicles[0].base_image_id;
	// Left hand arm
	image_id += 380;

	sub_98199C(image_id, al, cl, lengthX, lengthY, 90, height, boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ, get_current_rotation());

	uint32 seatImageId;

	if (vehicle != NULL && vehicle->restraints_position >= 64) {
		// Open Restraints
		image_id = (vehicle->restraints_position - 64) >> 6;
		image_id += direction * 3;
		image_id += rideEntry->vehicles[0].base_image_id;
		image_id += 64;
		seatImageId = image_id;
	}
	else {
		image_id = direction * 16;
		// Var_20 Rotation of seats
		image_id += seatRotation;
		image_id += rideEntry->vehicles[0].base_image_id;
		seatImageId = image_id;
	}

	image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	if (image_id == 0x20000000) {
		image_id =
			0xA0000000 |
			(ride->vehicle_colours[0].body_colour << 19) |
			(ride->vehicle_colours[0].trim_colour << 24);
	}
	image_id += seatImageId;

	rct_xyz16 seatCoords = {
		.x = al,
		.y = cl,
		.z = height
	};
	seatCoords.z += RCT2_ADDRESS(0x14280BC, sint16)[armRotation];

	assert(armRotation < sizeof(TopSpinSeatPositionOffset));
	switch (direction) {
	case 0:
		seatCoords.x -= TopSpinSeatPositionOffset[armRotation];
		break;
	case 1:
		seatCoords.y += TopSpinSeatPositionOffset[armRotation];
		break;
	case 2:
		seatCoords.x += TopSpinSeatPositionOffset[armRotation];
		break;
	case 3:
		seatCoords.y -= TopSpinSeatPositionOffset[armRotation];
		break;
	}

	RCT2_GLOBAL(0x014280B8, sint8) = (sint8)seatCoords.x;
	RCT2_GLOBAL(0x014280B9, sint8) = (sint8)seatCoords.y;
	RCT2_GLOBAL(0x014280BA, sint16) = seatCoords.z;

	sub_98199C(image_id, (sint8) seatCoords.x, (sint8) seatCoords.y, lengthX, lengthY, 90, seatCoords.z, boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ, get_current_rotation());

	rct_drawpixelinfo* dpi = RCT2_GLOBAL(0x140E9A8, rct_drawpixelinfo*);
	if (dpi->zoom_level < 2 && vehicle != NULL && vehicle->num_peeps != 0) {
		image_id =
			(vehicle->peep_tshirt_colours[0] << 19) |
			(vehicle->peep_tshirt_colours[1] << 24);
		image_id += seatImageId;
		image_id += 0xA0000000;
		image_id += 76;

		sub_98199C(image_id, (sint8) seatCoords.x, (sint8) seatCoords.y, lengthX, lengthY, 90, seatCoords.z, boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ, get_current_rotation());

		if (vehicle->num_peeps > 2) {
			image_id =
				(vehicle->peep_tshirt_colours[2] << 19) |
				(vehicle->peep_tshirt_colours[3] << 24);
			image_id += seatImageId;
			image_id += 0xA0000000;
			image_id += 152;

			sub_98199C(image_id, (sint8) seatCoords.x, (sint8) seatCoords.y, lengthX, lengthY, 90, seatCoords.z, boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ, get_current_rotation());
		}

		if (vehicle->num_peeps > 4) {
			image_id =
				(vehicle->peep_tshirt_colours[4] << 19) |
				(vehicle->peep_tshirt_colours[5] << 24);
			image_id += seatImageId;
			image_id += 0xA0000000;
			image_id += 228;

			sub_98199C(image_id, (sint8) seatCoords.x, (sint8) seatCoords.y, lengthX, lengthY, 90, seatCoords.z, boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ, get_current_rotation());
		}

		if (vehicle->num_peeps > 6) {
			image_id =
				(vehicle->peep_tshirt_colours[6] << 19) |
				(vehicle->peep_tshirt_colours[7] << 24);
			image_id += seatImageId;
			image_id += 0xA0000000;
			image_id += 304;

			sub_98199C(image_id, (sint8) seatCoords.x, (sint8) seatCoords.y, lengthX, lengthY, 90, seatCoords.z, boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ, get_current_rotation());
		}
	}

	image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	if (image_id == 0x20000000) {
		image_id =
			0xA0000000 |
			(ride->track_colour_main[0] << 19) |
			(ride->track_colour_additional[0] << 24);
	}

	image_id += var_1F;
	image_id += (direction & 1) * 48;
	image_id += rideEntry->vehicles[0].base_image_id;
	// Right hand arm
	image_id += 476;

	sub_98199C(image_id, al, cl, lengthX, lengthY, 90, height, boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ, get_current_rotation());

	image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	if (image_id == 0x20000000) {
		image_id =
			0xA0000000 |
			(ride->track_colour_main[0] << 19) |
			(ride->track_colour_supports[0] << 24);
	}

	image_id += (direction & 1) << 1;
	image_id += rideEntry->vehicles[0].base_image_id;
	// Right back bottom support
	image_id += 573;

	sub_98199C(image_id, al, cl, lengthX, lengthY, 90, height, boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ, get_current_rotation());

	RCT2_GLOBAL(0x009DE578, rct_map_element*) = curMapElement;
	RCT2_GLOBAL(RCT2_ADDRESS_PAINT_SETUP_CURRENT_TYPE, uint8) = VIEWPORT_INTERACTION_ITEM_RIDE;
}

/**
 *
 *  rct2: 0x0076693F
 */
static void top_spin_paint_tile_1(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	wooden_a_supports_paint_setup(direction & 1, 0, height, image_id, NULL);

	image_id = 22137 | RCT2_GLOBAL(0x00F44198, uint32);
	sub_98197C(image_id, 0, 0, 32, 32, 1, height, 0, 0, height, get_current_rotation());

	sint16 x = RCT2_GLOBAL(0x009DE56A, sint16), y = RCT2_GLOBAL(0x009DE56E, sint16);
	uint16 entranceLoc =
		((x / 32) + loc_7667AE[get_current_rotation()].x) |
		(((y / 32) + loc_7667AE[get_current_rotation()].y) << 8);

	uint8 entranceId = (mapElement->properties.track.sequence & 0x70) >> 4;
	rct_ride* ride = get_ride(rideIndex);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {
		image_id = 22141 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98199C(image_id, 0, 0, 32, 1, 7, height, 0, 2, height + 2, get_current_rotation());
	}

	entranceLoc =
		((x / 32) + loc_7667AC[get_current_rotation()].x) |
		(((y / 32) + loc_7667AC[get_current_rotation()].y) << 8);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {
		image_id = 22138 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98199C(image_id, 0, 0, 1, 32, 7, height, 0, 2, height + 2, get_current_rotation());
	}

	top_spin_paint_vehicle(32, 32, rideIndex, direction, height, mapElement);

	RCT2_GLOBAL(0x141E9B4, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9B6, uint16) = 32;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9CA, uint16) = 32;
	RCT2_GLOBAL(0x141E9CC, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9CE, uint16) = 32;
	RCT2_GLOBAL(0x141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D4, uint16) = 0xFFFF;

	height += 110;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/**
 *
 *  rct2: 0x00767033
 */
static void top_spin_paint_tile_2(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	wooden_a_supports_paint_setup(direction & 1, 0, height, image_id, NULL);

	image_id = 22137 | RCT2_GLOBAL(0x00F44198, uint32);
	sub_98197C(image_id, 0, 0, 32, 32, 1, height, 0, 0, height, get_current_rotation());

	sint16 x = RCT2_GLOBAL(0x009DE56A, sint16), y = RCT2_GLOBAL(0x009DE56E, sint16);
	uint16 entranceLoc =
		((x / 32) + loc_7667AC[get_current_rotation()].x) |
		(((y / 32) + loc_7667AC[get_current_rotation()].y) << 8);

	uint8 entranceId = (mapElement->properties.track.sequence & 0x70) >> 4;
	rct_ride* ride = get_ride(rideIndex);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {
		image_id = 22138 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98199C(image_id, 0, 0, 1, 32, 7, height, 2, 0, height + 2, get_current_rotation());
	}

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D4, uint16) = 0xFFFF;

	height += 110;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/**
 *
 *  rct2: 0x0076718D
 */
static void top_spin_paint_tile_4(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	wooden_a_supports_paint_setup(direction & 1, 0, height, image_id, NULL);

	image_id = 22137 | RCT2_GLOBAL(0x00F44198, uint32);
	sub_98197C(image_id, 0, 0, 32, 32, 1, height, 0, 0, height, get_current_rotation());

	sint16 x = RCT2_GLOBAL(0x009DE56A, sint16), y = RCT2_GLOBAL(0x009DE56E, sint16);
	uint16 entranceLoc =
		((x / 32) + loc_7667AE[get_current_rotation()].x) |
		(((y / 32) + loc_7667AE[get_current_rotation()].y) << 8);

	uint8 entranceId = (mapElement->properties.track.sequence & 0x70) >> 4;
	rct_ride* ride = get_ride(rideIndex);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {
		image_id = 22141 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98199C(image_id, 0, 0, 32, 1, 7, height, 0, 2, height + 2, get_current_rotation());
	}

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D4, uint16) = 0xFFFF;

	height += 110;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/**
 *
 *  rct2: 0x00766B4C
 */
static void top_spin_paint_tile_3(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	wooden_a_supports_paint_setup(direction & 1, 0, height, image_id, NULL);

	image_id = 22136 | RCT2_GLOBAL(0x00F44198, uint32);
	sub_98197C(image_id, 0, 0, 32, 32, 1, height, 0, 0, height, get_current_rotation());

	sint16 x = RCT2_GLOBAL(0x009DE56A, sint16), y = RCT2_GLOBAL(0x009DE56E, sint16);
	uint16 entranceLoc =
		((x / 32) + loc_7667AC[get_current_rotation()].x) |
		(((y / 32) + loc_7667AC[get_current_rotation()].y) << 8);

	uint8 entranceId = (mapElement->properties.track.sequence & 0x70) >> 4;
	rct_ride* ride = get_ride(rideIndex);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {
		image_id = 22138 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98199C(image_id, 0, 0, 1, 32, 7, height, 2, 0, height + 2, get_current_rotation());
	}

	entranceLoc =
		((x / 32) + loc_7667AC[(get_current_rotation() + 3) & 3].x) |
		(((y / 32) + loc_7667AC[(get_current_rotation() + 3) & 3].y) << 8);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {

		image_id = 22139 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98197C(image_id, 0, 0, 32, 1, 7, height, 0, 30, height + 2, get_current_rotation());
	}

	top_spin_paint_vehicle(32, -32, rideIndex, direction, height, mapElement);

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9BE, uint16) = 32;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9CC, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9CE, uint16) = 32;
	RCT2_GLOBAL(0x141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D4, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9D6, uint16) = 32;

	height += 110;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/**
 *
 *  rct2: 0x007672E7
 */
static void top_spin_paint_tile_5(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	wooden_a_supports_paint_setup(direction & 1, 0, height, image_id, NULL);

	image_id = 22136 | RCT2_GLOBAL(0x00F44198, uint32);
	sub_98197C(image_id, 0, 0, 32, 32, 1, height, 0, 0, height, get_current_rotation());

	sint16 x = RCT2_GLOBAL(0x009DE56A, sint16), y = RCT2_GLOBAL(0x009DE56E, sint16);
	uint8 entranceId = (mapElement->properties.track.sequence & 0x70) >> 4;
	rct_ride* ride = get_ride(rideIndex);

	uint16 entranceLoc =
		((x / 32) + loc_7667AC[(get_current_rotation() + 3) & 3].x) |
		(((y / 32) + loc_7667AC[(get_current_rotation() + 3) & 3].y) << 8);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {
		image_id = 22139 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98197C(image_id, 0, 0, 32, 1, 7, height, 0, 30, height + 2, get_current_rotation());
	}

	top_spin_paint_vehicle(0, -32, rideIndex, direction, height, mapElement);

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D4, uint16) = 0xFFFF;

	height += 112;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/**
 *
 *  rct2: 0x00766D09
 */
static void top_spin_paint_tile_6(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	wooden_a_supports_paint_setup(direction & 1, 0, height, image_id, NULL);

	image_id = 22135 | RCT2_GLOBAL(0x00F44198, uint32);
	sub_98197C(image_id, 0, 0, 32, 32, 1, height, 0, 0, height, get_current_rotation());

	sint16 x = RCT2_GLOBAL(0x009DE56A, sint16), y = RCT2_GLOBAL(0x009DE56E, sint16);
	uint16 entranceLoc =
		((x / 32) + loc_7667AE[get_current_rotation()].x) |
		(((y / 32) + loc_7667AE[get_current_rotation()].y) << 8);

	uint8 entranceId = (mapElement->properties.track.sequence & 0x70) >> 4;
	rct_ride* ride = get_ride(rideIndex);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {
		image_id = 22141 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98199C(image_id, 0, 0, 32, 1, 7, height, 0, 2, height + 2, get_current_rotation());
	}

	entranceLoc =
		((x / 32) + loc_7667AE[(get_current_rotation() + 1) & 3].x) |
		(((y / 32) + loc_7667AE[(get_current_rotation() + 1) & 3].y) << 8);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {

		image_id = 22140 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98197C(image_id, 0, 0, 1, 32, 7, height, 30, 2, height + 2, get_current_rotation());
	}

	top_spin_paint_vehicle(-32, 32, rideIndex, direction, height, mapElement);

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9BA, uint16) = 32;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9CA, uint16) = 32;
	RCT2_GLOBAL(0x141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D0, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9D2, uint16) = 32;
	RCT2_GLOBAL(0x141E9D4, uint16) = 0xFFFF;

	height += 110;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/**
 *
 *  rct2: 0x00766EC6
 */
static void top_spin_paint_tile_7(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	wooden_a_supports_paint_setup(direction & 1, 0, height, image_id, NULL);

	image_id = 22134 | RCT2_GLOBAL(0x00F44198, uint32);
	sub_98197C(image_id, 0, 0, 32, 32, 1, height, 0, 0, height, get_current_rotation());

	sint16 x = RCT2_GLOBAL(0x009DE56A, sint16), y = RCT2_GLOBAL(0x009DE56E, sint16);
	uint8 entranceId = (mapElement->properties.track.sequence & 0x70) >> 4;
	rct_ride* ride = get_ride(rideIndex);

	uint16 entranceLoc =
		((x / 32) + loc_7667AE[(get_current_rotation() + 1) & 3].x) |
		(((y / 32) + loc_7667AE[(get_current_rotation() + 1) & 3].y) << 8);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {

		image_id = 22140 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98197C(image_id, 0, 0, 1, 28, 7, height, 29, 0, height + 3, get_current_rotation());
	}

	entranceLoc =
		((x / 32) + loc_7667AC[(get_current_rotation() + 3) & 3].x) |
		(((y / 32) + loc_7667AC[(get_current_rotation() + 3) & 3].y) << 8);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {

		image_id = 22139 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98197C(image_id, 0, 0, 28, 1, 7, height, 0, 29, height + 3, get_current_rotation());
	}

	top_spin_paint_vehicle(-32, -32, rideIndex, direction, height, mapElement);

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9C2, uint16) = 32;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D0, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9D2, uint16) = 32;
	RCT2_GLOBAL(0x141E9D4, uint16) = height + 2;
	RCT2_GLOBAL(0x141E9D6, uint16) = 32;

	height += 110;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/**
 *
 *  rct2: 0x007673FA
 */
static void top_spin_paint_tile_8(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	wooden_a_supports_paint_setup(direction & 1, 0, height, image_id, NULL);

	image_id = 22135 | RCT2_GLOBAL(0x00F44198, uint32);
	sub_98197C(image_id, 0, 0, 32, 32, 1, height, 0, 0, height, get_current_rotation());

	sint16 x = RCT2_GLOBAL(0x009DE56A, sint16), y = RCT2_GLOBAL(0x009DE56E, sint16);
	uint8 entranceId = (mapElement->properties.track.sequence & 0x70) >> 4;
	rct_ride* ride = get_ride(rideIndex);

	uint16 entranceLoc =
		((x / 32) + loc_7667AE[(get_current_rotation() + 1) & 3].x) |
		(((y / 32) + loc_7667AE[(get_current_rotation() + 1) & 3].y) << 8);

	if (ride->entrances[entranceId] != entranceLoc && ride->exits[entranceId] != entranceLoc) {

		image_id = 22140 | RCT2_GLOBAL(0x00F441A0, uint32);
		sub_98197C(image_id, 0, 0, 1, 32, 7, height, 30, 0, height + 2, get_current_rotation());
	}
	top_spin_paint_vehicle(-32, 0, rideIndex, direction, height, mapElement);

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D4, uint16) = 0xFFFF;

	height += 112;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/**
 *
 *  rct2: 0x007667BC
 */
static void top_spin_paint_setup_rot_0(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	switch (trackSequence)
	{
	case 0:
		top_spin_paint_tile_0(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 1:
		top_spin_paint_tile_1(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 2:
		top_spin_paint_tile_2(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 3:
		top_spin_paint_tile_3(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 4:
		top_spin_paint_tile_4(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 5:
		top_spin_paint_tile_5(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 6:
		top_spin_paint_tile_6(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 7:
		top_spin_paint_tile_7(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 8:
		top_spin_paint_tile_8(rideIndex, trackSequence, direction, height, mapElement);
		break;
	}
	return;
}

/**
 *
 *  rct2: 0x007667EC
*/
static void top_spin_paint_setup_rot_1(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	switch (trackSequence)
	{
	case 0:
		top_spin_paint_tile_0(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 1:
		top_spin_paint_tile_3(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 2:
		top_spin_paint_tile_5(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 3:
		top_spin_paint_tile_7(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 4:
		top_spin_paint_tile_2(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 5:
		top_spin_paint_tile_8(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 6:
		top_spin_paint_tile_1(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 7:
		top_spin_paint_tile_6(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 8:
		top_spin_paint_tile_4(rideIndex, trackSequence, direction, height, mapElement);
		break;
	}
	return;
}

/**
 *
 *  rct2: 0x0076681C
*/
static void top_spin_paint_setup_rot_2(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	switch (trackSequence)
	{
	case 0:
		top_spin_paint_tile_0(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 1:
		top_spin_paint_tile_7(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 2:
		top_spin_paint_tile_8(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 3:
		top_spin_paint_tile_6(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 4:
		top_spin_paint_tile_5(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 5:
		top_spin_paint_tile_4(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 6:
		top_spin_paint_tile_3(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 7:
		top_spin_paint_tile_1(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 8:
		top_spin_paint_tile_2(rideIndex, trackSequence, direction, height, mapElement);
		break;
	}
	return;
}

/**
 *
 *  rct2: 0x0076684C
*/
static void top_spin_paint_setup_rot_3(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	switch (trackSequence)
	{
	case 0:
		top_spin_paint_tile_0(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 1:
		top_spin_paint_tile_6(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 2:
		top_spin_paint_tile_4(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 3:
		top_spin_paint_tile_1(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 4:
		top_spin_paint_tile_8(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 5:
		top_spin_paint_tile_2(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 6:
		top_spin_paint_tile_7(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 7:
		top_spin_paint_tile_3(rideIndex, trackSequence, direction, height, mapElement);
		break;
	case 8:
		top_spin_paint_tile_5(rideIndex, trackSequence, direction, height, mapElement);
		break;
	}
	return;
}

/* 0x0076659C */
TRACK_PAINT_FUNCTION get_track_paint_function_topspin(int trackType, int direction)
{
	switch (trackType) {
	case 123:
		switch (direction) {
		case 0: return top_spin_paint_setup_rot_0;
		case 1: return top_spin_paint_setup_rot_1;
		case 2: return top_spin_paint_setup_rot_2;
		case 3: return top_spin_paint_setup_rot_3;
		}
		break;
	}
	return NULL;
}

enum {
	SPR_MAZE_BASE_HEDGE = 21938,
	SPR_MAZE_BASE_BRICK = 21951,
	SPR_MAZE_BASE_ICE = 21964,
	SPR_MAZE_BASE_WOOD = 21977,
};

enum {
	SPR_MAZE_OFFSET_WALL_CENTER = 0,
	SPR_MAZE_OFFSET_WALL_INNER_NE_SW,
	SPR_MAZE_OFFSET_WALL_INNER_NW_SE,
	SPR_MAZE_OFFSET_WALL_TOP_LEFT,
	SPR_MAZE_OFFSET_WALL_TOP_RIGHT,
	SPR_MAZE_OFFSET_WALL_BOTTOM_RIGHT,
	SPR_MAZE_OFFSET_WALL_BOTTOM_LEFT,
	SPR_MAZE_OFFSET_COLUMN_CENTER,
	SPR_MAZE_OFFSET_COLUMN_TOP_RIGHT,
	SPR_MAZE_OFFSET_COLUMN_TOP_LEFT,
	SPR_MAZE_OFFSET_COLUMN_BOTTOM_LEFT,
	SPR_MAZE_OFFSET_COLUMN_BOTTOM_RIGHT,
	SPR_MAZE_OFFSET_COLUMN_CORNER,
};

/**
 * rct: 0x004ACF4A
 */
static void maze_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element *mapElement) {
	uint16 maze_entry = mapElement->properties.track.maze_entry;
	maze_entry = rol16(maze_entry, direction * 4);

	// draw ground
	int image_id = 2485 | RCT2_GLOBAL(0x00F441A0, uint32);
	sub_98196C(image_id, 0, 0, 32, 32, 0, height, get_current_rotation());

	wooden_a_supports_paint_setup(direction & 1, 0, height, RCT2_GLOBAL(0x00F441A4, uint32), NULL);

	RCT2_GLOBAL(0x0141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9D4, uint16) = 0xFFFF;

	int base_image_id;
	switch (get_ride(rideIndex)->track_colour_supports[0]) {
		case 0: base_image_id = SPR_MAZE_BASE_BRICK; break;
		case 1: base_image_id = SPR_MAZE_BASE_HEDGE; break;
		case 2: base_image_id = SPR_MAZE_BASE_ICE; break;
		case 3: base_image_id = SPR_MAZE_BASE_WOOD; break;
	}

	base_image_id |= RCT2_GLOBAL(0x00F441A0, uint32);


	image_id = base_image_id + SPR_MAZE_OFFSET_WALL_CENTER;
	if (maze_entry & (1 << 3))
		sub_98197C(image_id, 2, 2, 10, 10, 9, height, 3, 3, height + 2, get_current_rotation());

	if (maze_entry & (1 << 7))
		sub_98197C(image_id, 2, 18, 10, 10, 9, height, 3, 19, height + 2, get_current_rotation());

	if (maze_entry & (1 << 11))
		sub_98197C(image_id, 18, 18, 10, 10, 9, height, 19, 19, height + 2, get_current_rotation());

	if (maze_entry & (1 << 15))
		sub_98197C(image_id, 18, 2, 10, 10, 9, height, 19, 3, height + 2, get_current_rotation());


	image_id = base_image_id + SPR_MAZE_OFFSET_WALL_TOP_LEFT;
	if (maze_entry & (1 << 0))
		sub_98197C(image_id, 2, 0, 10, 1, 9, height, 3, 1, height + 2, get_current_rotation());

	if (maze_entry & (1 << 13))
		sub_98197C(image_id, 18, 0, 10, 1, 9, height, 19, 1, height + 2, get_current_rotation());


	image_id = base_image_id + SPR_MAZE_OFFSET_WALL_BOTTOM_RIGHT;
	if (maze_entry & (1 << 5))
		sub_98197C(image_id, 2, 30, 10, 1, 9, height, 3, 30, height + 2, get_current_rotation());

	if (maze_entry & (1 << 8))
		sub_98197C(image_id, 18, 30, 10, 1, 9, height, 19, 30, height + 2, get_current_rotation());


	image_id = base_image_id + SPR_MAZE_OFFSET_WALL_TOP_RIGHT;
	if (maze_entry & (1 << 1))
		sub_98197C(image_id, 0, 2, 1, 10, 9, height, 1, 3, height + 2, get_current_rotation());

	if (maze_entry & (1 << 4))
		sub_98197C(image_id, 0, 18, 1, 10, 9, height, 1, 19, height + 2, get_current_rotation());


	image_id = base_image_id + SPR_MAZE_OFFSET_WALL_BOTTOM_LEFT;
	if (maze_entry & (1 << 12))
		sub_98197C(image_id, 30, 2, 1, 10, 9, height, 30, 3, height + 2, get_current_rotation());

	if (maze_entry & (1 << 9))
		sub_98197C(image_id, 30, 18, 1, 10, 9, height, 30, 19, height + 2, get_current_rotation());


	image_id = base_image_id + SPR_MAZE_OFFSET_WALL_INNER_NE_SW;
	if (maze_entry & (1 << 2))
		sub_98197C(image_id, 2, 14, 10, 4, 9, height, 3, 14, height + 2, get_current_rotation());

	if (maze_entry & (1 << 10))
		sub_98197C(image_id, 18, 14, 10, 4, 9, height, 19, 14, height + 2, get_current_rotation());


	image_id = base_image_id + SPR_MAZE_OFFSET_WALL_INNER_NW_SE;
	if (maze_entry & (1 << 14))
		sub_98197C(image_id, 14, 2, 4, 10, 9, height, 14, 3, height + 2, get_current_rotation());

	if (maze_entry & (1 << 6))
		sub_98197C(image_id, 14, 18, 4, 10, 9, height, 14, 19, height + 2, get_current_rotation());


	image_id = base_image_id + SPR_MAZE_OFFSET_COLUMN_CORNER;
	if (maze_entry & (1 << 0 | 1 << 1))
		sub_98197C(image_id, 0, 0, 1, 1, 9, height, 1, 1, height + 2, get_current_rotation());

	if (maze_entry & (1 << 4 | 1 << 5))
		sub_98197C(image_id, 0, 30, 1, 1, 9, height, 1, 30, height + 2, get_current_rotation());

	if (maze_entry & (1 << 8 | 1 << 9))
		sub_98197C(image_id, 30, 30, 1, 1, 9, height, 30, 30, height + 2, get_current_rotation());

	if (maze_entry & (1 << 12 | 1 << 13))
		sub_98197C(image_id, 30, 0, 1, 1, 9, height, 30, 1, height + 2, get_current_rotation());


	if (maze_entry & (1 << 0 | 1 << 13 | 1 << 14))
		sub_98197C(base_image_id + SPR_MAZE_OFFSET_COLUMN_TOP_LEFT, 14, 0, 2, 1, 9, height, 15, 1, height + 2, get_current_rotation());


	if (maze_entry & (1 << 5 | 1 << 6 | 1 << 8))
		sub_98197C(base_image_id + SPR_MAZE_OFFSET_COLUMN_BOTTOM_RIGHT, 14, 30, 2, 1, 9, height, 15, 30, height + 2, get_current_rotation());


	if (maze_entry & (1 << 1 | 1 << 2 | 1 << 4))
		sub_98197C(base_image_id + SPR_MAZE_OFFSET_COLUMN_TOP_RIGHT, 0, 14, 1, 2, 9, height, 1, 15, height + 2, get_current_rotation());


	if (maze_entry & (1 << 9 | 1 << 10 | 1 << 12))
		sub_98197C(base_image_id + SPR_MAZE_OFFSET_COLUMN_BOTTOM_LEFT, 30, 14, 1, 2, 9, height, 30, 15, height + 2, get_current_rotation());
	

	if (maze_entry & (1 << 2 | 1 << 6 | 1 << 10 | 1 << 14)) {
		sub_98197C(base_image_id + SPR_MAZE_OFFSET_COLUMN_CENTER, 14, 14, 2, 2, 8, height, 15, 15, height + 2, get_current_rotation());

		RCT2_GLOBAL(0x141E9C4, uint16) = height + 12;
		RCT2_GLOBAL(0x141E9C6, uint8) = 0x20;
	}

	height += 32;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 0x20;
	}
}

/**
 * rct2: 0x008A81E8
 */
TRACK_PAINT_FUNCTION get_track_paint_function_maze(int trackType, int direction) {
	if (trackType != 101) {
		return NULL;
	}

	return maze_paint_setup;
}

/**
 *
 *  rct2: 0x00761378
 *  rct2: 0x007614DB
 *  rct2: 0x0076163F
 *  rct2: 0x007617A5
 */
static void shop_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement)
{
	bool hasSupports = wooden_a_supports_paint_setup(direction & 1, 0, height, RCT2_GLOBAL(0x00F441A4, uint32), NULL);

	RCT2_GLOBAL(0x0141E9D0, sint16) = -1;
	RCT2_GLOBAL(0x0141E9C4, sint16) = -1;
	RCT2_GLOBAL(0x0141E9CC, sint16) = -1;
	RCT2_GLOBAL(0x0141E9B8, sint16) = -1;
	RCT2_GLOBAL(0x0141E9BC, sint16) = -1;
	RCT2_GLOBAL(0x0141E9B4, sint16) = -1;
	RCT2_GLOBAL(0x0141E9C0, sint16) = -1;
	RCT2_GLOBAL(0x0141E9C8, sint16) = -1;
	RCT2_GLOBAL(0x0141E9D4, sint16) = -1;

	rct_ride *ride = get_ride(rideIndex);
	rct_ride_entry *rideEntry = get_ride_entry(ride->subtype);
	rct_ride_entry_vehicle *firstVehicleEntry = &rideEntry->vehicles[0];

	uint32 imageId = RCT2_GLOBAL(0x00F44198, uint32);
	if (imageId & 0x80000000) {
		imageId &= 0x60FFFFFF;
	}
	imageId += firstVehicleEntry->base_image_id;
	imageId += direction;

	sint16 height16 = (sint16)height;
	int rotation = get_current_rotation();
	if (hasSupports) {
		uint32 foundationImageId = RCT2_GLOBAL(0x00F441A4, uint32);
		foundationImageId |= 3395;
		sub_98197C(foundationImageId, 0, 0, 28, 28, 45, height, 2, 2, height16, rotation);

		sub_98199C(imageId, 0, 0, 28, 28, 45, height, 2, 2, height16, rotation);
	} else {
		sub_98197C(imageId, 0, 0, 28, 28, 45, height, 2, 2, height16, rotation);
	}

	height16 += 48;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height16) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height16;
		RCT2_GLOBAL(0x00141E9DA, uint8) = 32;
	}
}

/* 0x00761160 */
TRACK_PAINT_FUNCTION get_track_paint_function_shop(int trackType, int direction)
{
	switch (trackType) {
	case 118:
	case 121: return shop_paint_setup;
	}
	return NULL;
}

/**
 *
 *  rct2: 0x00763234
 *  rct2: 0x0076338C
 *  rct2: 0x00762F50
 *  rct2: 0x007630DE
 */
static void facility_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement)
{
	bool hasSupports = wooden_a_supports_paint_setup(direction & 1, 0, height, RCT2_GLOBAL(0x00F441A4, uint32), NULL);

	RCT2_GLOBAL(0x0141E9D0, sint16) = -1;
	RCT2_GLOBAL(0x0141E9C4, sint16) = -1;
	RCT2_GLOBAL(0x0141E9CC, sint16) = -1;
	RCT2_GLOBAL(0x0141E9B8, sint16) = -1;
	RCT2_GLOBAL(0x0141E9BC, sint16) = -1;
	RCT2_GLOBAL(0x0141E9B4, sint16) = -1;
	RCT2_GLOBAL(0x0141E9C0, sint16) = -1;
	RCT2_GLOBAL(0x0141E9C8, sint16) = -1;
	RCT2_GLOBAL(0x0141E9D4, sint16) = -1;

	rct_ride *ride = get_ride(rideIndex);
	rct_ride_entry *rideEntry = get_ride_entry(ride->subtype);
	rct_ride_entry_vehicle *firstVehicleEntry = &rideEntry->vehicles[0];

	uint32 imageId = RCT2_GLOBAL(0x00F44198, uint32);
	imageId |= firstVehicleEntry->base_image_id;
	imageId += (direction + 2) & 3;

	sint16 height16 = (sint16)height;
	int rotation = get_current_rotation();
	int lengthX = (direction & 1) == 0 ? 28 : 2;
	int lengthY = (direction & 1) == 0 ? 2 : 28;
	if (hasSupports) {
		uint32 foundationImageId = RCT2_GLOBAL(0x00F441A4, uint32);
		foundationImageId |= 3395;
		sub_98197C(foundationImageId, 0, 0, lengthX, lengthY, 29, height, direction == 3 ? 28 : 2, direction == 0 ? 28 : 2, height16, rotation);

		// Door image or base
		sub_98199C(imageId, 0, 0, lengthX, lengthY, 29, height, direction == 3 ? 28 : 2, direction == 0 ? 28 : 2, height16, rotation);
	} else {
		// Door image or base
		sub_98197C(imageId, 0, 0, lengthX, lengthY, 29, height, direction == 3 ? 28 : 2, direction == 0 ? 28 : 2, height16, rotation);
	}

	// Base image if door was drawn
	if (direction == 1) {
		imageId += 2;
		sub_98197C(imageId, 0, 0, 2, 28, 29, height, 28, 2, height16, rotation);
	} else if (direction == 2) {
		imageId += 4;
		sub_98197C(imageId, 0, 0, 28, 2, 29, height, 2, 28, height16, rotation);
	}

	height16 += 32;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height16) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height16;
		RCT2_GLOBAL(0x00141E9DA, uint8) = 32;
	}
}

enum {
	SPR_JUNIOR_RC_FLAT_NE_SW = 27807,
	SPR_JUNIOR_RC_FLAT_NW_SE = 27808,
	SPR_JUNIOR_RC_25_DEG_NE_SW = 27809,
	SPR_JUNIOR_RC_FLAT_TO_25_DEG_UP_NE_SW = 27811,
	SPR_JUNIOR_RC_25_DEG_SW_NE = 27813,
	SPR_JUNIOR_RC_FLAT_TO_25_DEG_UP_SW_NE = 27815,
	SPR_JUNIOR_RC_25_DEG_UP_TO_FLAT_SW_NE = 27817,
	SPR_JUNIOR_RC_25_DEG_UP_TO_FLAT_NE_SW = 27819,
	SPR_JUNIOR_RC_25_DEG_NW_SE = 27821,
	SPR_JUNIOR_RC_25_DEG_SE_NW = 27823,
	SPR_JUNIOR_RC_FLAT_TO_25_DEG_UP_NW_SE = 27825,
	SPR_JUNIOR_RC_FLAT_TO_25_DEG_UP_SE_NW = 27827,
	SPR_JUNIOR_RC_25_DEG_UP_TO_FLAT_SE_NW = 27829,
	SPR_JUNIOR_RC_25_DEG_UP_TO_FLAT_NW_SE = 27831,
	SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_SW_NE = 27997,
	SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_NW_SE = 27998,
	SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_NE_SW = 27999,
	SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_SE_NW = 28000,
	SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_SW_NE = 28001,
	SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_NW_SE = 28002,
	SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_NE_SW = 28003,
	SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_SE_NW = 28004,
	SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_SW_NE_FRONT = 28005,
	SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_NW_SE_FRONT = 28006,
	SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_NE_SW_FRONT = 28007,
	SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_SE_NW_FRONT = 28008,
};

/* 0x00762D44 */
TRACK_PAINT_FUNCTION get_track_paint_function_facility(int trackType, int direction)
{
	switch (trackType) {
	case 118: return facility_paint_setup;
	}
	return NULL;
}

/**
 * rct2: 0x00761378, 0x007614DB, 0x0076163F, 0x007617A5
 */
static void ride_50_52_53_54_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element *mapElement) {
	bool supportsDrawn = wooden_a_supports_paint_setup(direction & 1, 0, height, RCT2_GLOBAL(0x00F441A4, uint32), NULL);

	RCT2_GLOBAL(0x0141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x0141E9D4, uint16) = 0xFFFF;

	rct_ride *ride = get_ride(rideIndex);

	rct_ride_entry *rideEntry = get_ride_entry(ride->subtype);

	uint32 image_id = RCT2_GLOBAL(0x00F44198, uint32);
	if (image_id & 0x80000000) {
		image_id &= 0x60FFFFFF;
	}

	image_id += rideEntry->vehicles[0].base_image_id;
	image_id += direction;
	if (supportsDrawn) {
		uint32 ebx = (direction & 1 ? 3396 : 3395) | RCT2_GLOBAL(0x00F441A4, uint32);
		sub_98197C(ebx, 0, 0, 28, 28, 45, height, 2, 2, height, get_current_rotation());

		sub_98199C(image_id, 0, 0, 28, 28, 45, height, 2, 2, height, get_current_rotation());
	} else {
		sub_98197C(image_id, 0, 0, 28, 28, 45, height, 2, 2, height, get_current_rotation());
	}

	height += 48;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x00141E9DA, uint8) = 0x20;
	}
}

/**
 * rct2: 0x00761160
 */
TRACK_PAINT_FUNCTION get_track_paint_function_50_52_53_54(int trackType, int direction) {
	switch (trackType) {
		case 118:
		case 121:
			return ride_50_52_53_54_paint_setup;
	}
	return NULL;
}

/* Supports are only placed every 2 tiles for flat pieces*/
bool junior_rc_support_should_place() {
	if ((RCT2_GLOBAL(0x009DE56A, sint16) & (1 << 5)) && (RCT2_GLOBAL(0x009DE56E, sint16)& (1 << 5)))
		return true;

	if ((!(RCT2_GLOBAL(0x009DE56A, sint16) & (1 << 5))) && (!(RCT2_GLOBAL(0x009DE56E, sint16)& (1 << 5))))
		return true;

	return false;
}

/* rct2: 0x00518394 */
void junior_rc_flat_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement){

	uint32 image_id = SPR_JUNIOR_RC_FLAT_NE_SW | RCT2_GLOBAL(0x00F44198, uint32);
	image_id += direction & 1;

	if (mapElement->type & (1 << 7)) {
		image_id += 106;
	}

	//di
	uint8 lengthX = direction & 1 ? 20 : 32;
	//si
	uint8 lengthY = direction & 1 ? 32 : 20;

	//al
	sint8 offsetX = direction & 1 ? 6 : 0;
	//cl
	sint8 offsetY = direction & 1 ? 0 : 6;

	sub_98196C(image_id, offsetX, offsetY, lengthX, lengthY, 1, height, get_current_rotation());

	if (junior_rc_support_should_place()) {
		int edi = direction & 1 ? 2 : 1;
		metal_a_supports_paint_setup(edi, 4, 0, height, RCT2_GLOBAL(0x00F4419C, uint32));
	}

	RCT2_GLOBAL(0x00141E9C4, uint16) = 0xFFFF;
	uint32 eax = 0xFFFF0000;
	eax |= ((height & 0x0FFF) >> 4);

	if (direction & 1) {
		RCT2_GLOBAL(0x00141E9D4, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9C8, uint16) = 0xFFFF;

		RCT2_ADDRESS(0x009E30B6, uint32)[RCT2_GLOBAL(0x141F56B, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56B, uint8)++;
	}
	else {
		RCT2_GLOBAL(0x00141E9D0, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9CC, uint16) = 0xFFFF;

		RCT2_ADDRESS(0x009E3138, uint32)[RCT2_GLOBAL(0x141F56A, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56A, uint8)++;
	}

	height += 32;
	if (RCT2_GLOBAL(0x141E9D8, sint16) < height) {
		RCT2_GLOBAL(0x141E9D8, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

enum {
	// ENTRANCE means entrance at this side of platform
	// Fenced means fencing at end of platform (i.e. for begin station pieces)
	SPR_STATION_PLATFORM_ENTRANCE_NE_SW = 22362,
	SPR_STATION_PLATFORM_ENTRANCE_NW_SE = 22363,

	SPR_STATION_PLATFORM_NE_SW = 22364,
	SPR_STATION_PLATFORM_NW_SE = 22365,

	SPR_STATION_PLATFORM_FENCED_NE_SW = 22366,
	SPR_STATION_PLATFORM_FENCED_NW_SE = 22367,

	SPR_STATION_PLATFORM_FENCED_ENTRANCE_NE_SW = 22368,
	SPR_STATION_PLATFORM_FENCED_ENTRANCE_NW_SE = 22369,

	SPR_STATION_PLATFORM_LIGHTS_RED_NE_SW = 22380,
	SPR_STATION_PLATFORM_LIGHTS_RED_NW_SE = 22381,
	SPR_STATION_PLATFORM_LIGHTS_GREEN_NE_SW = 22382,
	SPR_STATION_PLATFORM_LIGHTS_GREEN_NW_SE = 22383,

	SPR_STATION_PLATFORM_ENTRANCE_LIGHTS_RED_NE_SW = 22388,
	SPR_STATION_PLATFORM_ENTRANCE_LIGHTS_RED_NW_SE = 22389,
	SPR_STATION_PLATFORM_ENTRANCE_LIGHTS_GREEN_NE_SW = 22390,
	SPR_STATION_PLATFORM_ENTRANCE_LIGHTS_GREEN_NW_SE = 22391,
};

bool station_platform_has_entrance(rct_ride* ride, uint8 direction, rct_map_element* mapElement) {
	int x = RCT2_GLOBAL(0x009DE56A, sint16) / 32;
	int y = RCT2_GLOBAL(0x009DE56E, sint16) / 32;
	uint16 entranceLoc =
		(x + loc_7667AE[(get_current_rotation() - (direction & 1)) & 3].x) |
		((y + loc_7667AE[(get_current_rotation() - (direction & 1)) & 3].y) << 8);

	uint8 entranceId = (mapElement->properties.track.sequence & 0x70) >> 4;
	if (ride->entrances[entranceId] == entranceLoc)
		return true;

	if (ride->exits[entranceId] == entranceLoc)
		return true;

	return false;
}

void station_platform_back_paint_setup(rct_ride* ride, uint8 direction, int height, rct_map_element* mapElement, bool lights) {
	uint32 image_id = RCT2_GLOBAL(0x0F4419C, uint32);

	bool showLights = (direction == 0 || direction == 3) && lights;

	image_id |= showLights ? SPR_STATION_PLATFORM_ENTRANCE_LIGHTS_RED_NE_SW : SPR_STATION_PLATFORM_ENTRANCE_NE_SW;
	image_id += direction & 1;

	if (!station_platform_has_entrance(ride,direction,mapElement)){
		image_id += showLights ? -8 : 2;

		RCT2_GLOBAL(0x00F441E8, uint32)++;
	}

	if (showLights) {
		if (mapElement->properties.track.sequence & (1 << 7)) {
			image_id += 2;
		}
	}

	uint8 lengthX = direction & 1 ? 8 : 32;
	uint8 lengthY = direction & 1 ? 32 : 8;

	sub_98196C(image_id, 0, 0, lengthX, lengthY, 1, height + 5, get_current_rotation());
}

void station_platform_begin_back_paint_setup(rct_ride* ride, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x0F4419C, uint32);

	bool showFence = (direction == 1 || direction == 2);

	image_id |= showFence ? SPR_STATION_PLATFORM_FENCED_ENTRANCE_NE_SW : SPR_STATION_PLATFORM_ENTRANCE_NE_SW;
	image_id += direction & 1;

	if (!station_platform_has_entrance(ride, direction, mapElement)) {
		image_id += showFence ? -2 : 2;

		RCT2_GLOBAL(0x00F441E8, uint32)++;
	}

	uint8 lengthX = direction & 1 ? 8 : 32;
	uint8 lengthY = direction & 1 ? 32 : 8;

	sub_98196C(image_id, 0, 0, lengthX, lengthY, 1, height + 5, get_current_rotation());
}

void station_platform_front_paint_setup(uint8 direction, int height, rct_map_element* mapElement, bool lights) {
	uint32 image_id = RCT2_GLOBAL(0x0F4419C, uint32);

	bool showLights = (direction == 0 || direction == 3) && lights;

	image_id |= showLights ? SPR_STATION_PLATFORM_ENTRANCE_LIGHTS_RED_NE_SW : SPR_STATION_PLATFORM_ENTRANCE_NE_SW;
	image_id += direction & 1;

	if (showLights) {
		if (mapElement->properties.track.sequence & (1 << 7)) {
			image_id += 2;
		}
	}

	sint8 offsetX = direction & 1 ? 24 : 0;
	sint8 offsetY = direction & 1 ? 0 : 24;
	uint8 lengthX = direction & 1 ? 8 : 32;
	uint8 lengthY = direction & 1 ? 32 : 8;

	sub_98196C(image_id, offsetX, offsetY, lengthX, lengthY, 1, height + 5, get_current_rotation());
}

void station_platform_begin_front_paint_setup(uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id = RCT2_GLOBAL(0x0F4419C, uint32);

	bool showLights = (direction == 0 || direction == 3);

	image_id |= showLights ? SPR_STATION_PLATFORM_ENTRANCE_NE_SW : SPR_STATION_PLATFORM_FENCED_ENTRANCE_NE_SW;
	image_id += direction & 1;

	sint8 offsetX = direction & 1 ? 24 : 0;
	sint8 offsetY = direction & 1 ? 0 : 24;
	uint8 lengthX = direction & 1 ? 8 : 32;
	uint8 lengthY = direction & 1 ? 32 : 8;

	sub_98196C(image_id, offsetX, offsetY, lengthX, lengthY, 1, height + 5, get_current_rotation());
}

void station_platform_fence_front_paint_setup(uint8 direction, int height, bool lights) {
	uint32 image_id = RCT2_GLOBAL(0x0F4419C, uint32);
	bool showLights = (direction == 1 || direction == 2) && lights;

	image_id |= showLights ? 22386 : 22370;
	image_id += direction & 1;

	uint8 offsetX = direction & 1 ? 31 : 0;
	uint8 offsetY = direction & 1 ? 0 : 31;
	sint8 lengthX = direction & 1 ? 1 : 32;
	sint8 lengthY = direction & 1 ? 32 : 1;

	sub_98196C(image_id, offsetX, offsetY, lengthX, lengthY, 7, height + 7, get_current_rotation());
}

void station_platform_begin_fence_front_paint_setup(uint8 direction, int height) {
	uint32 image_id = RCT2_GLOBAL(0x0F4419C, uint32);
	bool showFenceEnd = (direction == 0 || direction == 3);

	image_id |= showFenceEnd ? 22372 : 22370;
	image_id += direction & 1;

	uint8 offsetX = direction & 1 ? 31 : 0;
	uint8 offsetY = direction & 1 ? 0 : 31;
	sint8 lengthX = direction & 1 ? 1 : 32;
	sint8 lengthY = direction & 1 ? 32 : 1;

	sub_98196C(image_id, offsetX, offsetY, lengthX, lengthY, 7, height + 7, get_current_rotation());
}

void station_platform_fence_back_paint_setup(uint8 direction, int height, bool lights) {
	bool showLights = (direction == 1 || direction == 2) && lights;
	if (!showLights)
		return;

	uint32 image_id = RCT2_GLOBAL(0x00F4419C, uint32);
	image_id |= direction & 1 ? 22385 : 22384;
	uint8 offsetX = direction & 1 ? 0 : 31;
	uint8 offsetY = direction & 1 ? 31 : 0;
	sint8 lengthX = direction & 1 ? 8 : 1;
	sint8 lengthY = direction & 1 ? 1 : 8;
	sub_98196C(image_id, offsetX, offsetY, lengthX, lengthY, 7, height + 7, get_current_rotation());
}

void station_platform_begin_fence_back_paint_setup(uint8 direction, int height) {
	bool showFence = (direction == 0 || direction == 3);
	if (!showFence)
		return;

	uint32 image_id = RCT2_GLOBAL(0x00F4419C, uint32);
	image_id |= direction & 1 ? 22375 : 22374;
	uint8 offsetX = direction & 1 ? 0 : 31;
	uint8 offsetY = direction & 1 ? 31 : 0;
	sint8 lengthX = direction & 1 ? 8 : 1;
	sint8 lengthY = direction & 1 ? 1 : 8;
	sub_98196C(image_id, offsetX, offsetY, lengthX, lengthY, 7, height + 7, get_current_rotation());
}

void station_platform_entrance_style_back_paint_setup(uint8 direction, int height) {
	// Note this was missing in original.
	// would cause small graphical glitch underground
	if (!(RCT2_GLOBAL(0x0141E9DB, uint8) & 3))
		return;
	
	uint32 image_id = RCT2_GLOBAL(0x00F441E8, uint32);
	// Entrance style??
	if (image_id > 32) {
		if (!(image_id & (1 << 30))) {
			// Draw support
			image_id |= RCT2_GLOBAL(0x0F44198, uint32);
			if (direction & 1)
				image_id += 3;

			sint16 lengthX = direction & 1 ? 1 : 30;
			sint16 lengthY = direction & 1 ? 30 : 1;
			sub_98197C(image_id, 0, 0, lengthX, lengthY, 22, height, direction & 1 ? 0 : 1, direction & 1 ? 1 : 0, height + 1, get_current_rotation());
		}
		else {
			if (direction & 1)
				image_id += 3;

			sint16 lengthX = direction & 1 ? 1 : 30;
			sint16 lengthY = direction & 1 ? 30 : 1;

			sub_98197C(image_id & ~(1 << 30), 0, 0, lengthX, lengthY, 22, height, direction & 1 ? 0 : 1, direction & 1 ? 1 : 0, height + 1, get_current_rotation());

			// Just colour 1
			// Branch before on impossible situation RCT2_GLOBAL(0x0F44198, uint32) & 0xF80000 > 0x1000000
			image_id |= RCT2_GLOBAL(0x0F44198, uint32) & 0xF80000;
			image_id += 12;
			image_id += 0x3800000;

			sub_98199C(image_id, 0, 0, lengthX, lengthY, 1, height, direction & 1 ? 0 : 1, direction & 1 ? 1 : 0, height + 1, get_current_rotation());
		}
	}
	
}

void station_platform_entrance_style_front_paint_setup(uint8 direction, int height) {
	//51595d
	if (RCT2_GLOBAL(0x0141E9DB, uint8) & 3) {
		uint32 image_id = RCT2_GLOBAL(0x00F441E4, uint32);
		if (image_id >= 32) {

			if (image_id & (1 << 30)) {
				image_id &= ~(1 << 30);
				image_id += direction & 1 ? 5 : 2;

				sub_98197C(image_id, 0, 0, 32, 32, 0, height, 0, 0, height + 23, get_current_rotation());

				image_id = RCT2_GLOBAL(0x00F441E4, uint32);
				// Just colour 1
				// Branch before on impossible situation RCT2_GLOBAL(0x0F44198, uint32) & 0xF80000 > 0x1000000
				image_id |= RCT2_GLOBAL(0x0F44198, uint32) & 0xF80000;
				image_id += 0x3800000;
				image_id += 12 + ((direction & 1) ? 5 : 2);

				sub_98199C(image_id, 0, 0, 32, 32, 0, height, 0, 0, height + 23, get_current_rotation());

			}
			else {
				image_id |= RCT2_GLOBAL(0x00F44198, uint32);
				image_id += direction & 1 ? 5 : 2;
				RCT2_GLOBAL(0x009DEA52, sint16) = 0;
				RCT2_GLOBAL(0x009DEA54, sint16) = 0;
				RCT2_GLOBAL(0x009DEA56, sint16) = height + 23;

				sub_98197C(image_id, 0, 0, 32, 32, 0, height, 0, 0, height + 23, get_current_rotation());
			}

		}
	}
}

/* rct2: 0x00515629 */
void junior_rc_end_station_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement){
	rct_ride* ride = get_ride(rideIndex);

	uint8 entrance_style = ride->entrance_style;

	uint32 image_id = RideEntranceDefinitions[entrance_style].station_image_id;
	if (!(RCT2_GLOBAL(0x00F441A0, uint32) & (1 << 29))) {
		image_id &= 0x7FFFF;
	}

	RCT2_GLOBAL(0x00F441E8, uint32) = image_id;
	RCT2_GLOBAL(0x00F441E4, uint32) = image_id;

	image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	image_id |= 22428;
	image_id += direction & 1;

	//di
	uint8 lengthX = direction & 1 ? 28 : 32;
	//si
	uint8 lengthY = direction & 1 ? 32 : 28;

	sub_98197C(image_id, 0, 0, lengthX, lengthY, 1, height - 2, direction & 1 ? 2 : 0, direction & 1 ? 0 : 2, height, get_current_rotation());

	image_id = RCT2_GLOBAL(0x00F44198, uint32);
	image_id |= 28193;
	image_id += direction & 1;

	if (mapElement->flags & MAP_ELEMENT_FLAG_BROKEN) {
		image_id += 2;
	}

	lengthX = direction & 1 ? 20 : 32;
	lengthY = direction & 1 ? 32 : 20;
	sint8 offsetX = direction & 1 ? 6 : 0;
	sint8 offsetY = direction & 1 ? 0 : 6;

	sub_98199C(image_id, offsetX, offsetY, lengthX, lengthY, 1, height, 0, 0, height, get_current_rotation());

	uint8 bl = direction & 1 ? 6 : 5;
	metal_a_supports_paint_setup(3, bl, 0, height, RCT2_GLOBAL(0x00F4419C, uint32));

	bl = direction & 1 ? 7 : 8;
	metal_a_supports_paint_setup(3, bl, 0, height, RCT2_GLOBAL(0x00F4419C, uint32));

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D4, uint16) = 0xFFFF;

	uint32 eax = 0xFFFF0000;
	eax |= ((height & 0x0FFF) >> 4);
	eax |= (6 << 8);
	if (direction & 1) {
		RCT2_ADDRESS(0x009E30B6, uint32)[RCT2_GLOBAL(0x141F56B, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56B, uint8)++;
	}
	else {
		RCT2_ADDRESS(0x009E3138, uint32)[RCT2_GLOBAL(0x141F56A, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56A, uint8)++;
	}

	station_platform_back_paint_setup(ride, direction, height, mapElement, true);
	//0x0515791

	station_platform_entrance_style_back_paint_setup(direction, height);
	
	station_platform_front_paint_setup(direction, height, mapElement, true);

	// Note in vanilla this draw call was after underground check
	// so it would not be displayed when underground
	station_platform_fence_front_paint_setup(direction, height, true);

	station_platform_entrance_style_front_paint_setup(direction, height);

	station_platform_fence_back_paint_setup(direction, height, true);

	height += 25;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x00141E9DA, uint8) = 32;
	}
}

/* rct2: 0x00514D22 */
void junior_rc_begin_station_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	rct_ride* ride = get_ride(rideIndex);

	uint8 entrance_style = ride->entrance_style;

	uint32 image_id = RideEntranceDefinitions[entrance_style].station_image_id;
	if (!(RCT2_GLOBAL(0x00F441A0, uint32) & (1 << 29))) {
		image_id &= 0x7FFFF;
	}

	RCT2_GLOBAL(0x00F441E8, uint32) = image_id;
	RCT2_GLOBAL(0x00F441E4, uint32) = image_id;

	image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	image_id |= 22428;
	image_id += direction & 1;

	//di
	uint8 lengthX = direction & 1 ? 28 : 32;
	//si
	uint8 lengthY = direction & 1 ? 32 : 28;

	sub_98197C(image_id, 0, 0, lengthX, lengthY, 1, height - 2, direction & 1 ? 2 : 0, direction & 1 ? 0 : 2, height, get_current_rotation());

	image_id = RCT2_GLOBAL(0x00F44198, uint32);
	image_id |= 28189;
	image_id += direction & 1;

	lengthX = direction & 1 ? 20 : 32;
	lengthY = direction & 1 ? 32 : 20;
	sint8 offsetX = direction & 1 ? 6 : 0;
	sint8 offsetY = direction & 1 ? 0 : 6;

	sub_98199C(image_id, offsetX, offsetY, lengthX, lengthY, 1, height, 0, 0, height, get_current_rotation());

	uint8 bl = direction & 1 ? 6 : 5;

	metal_a_supports_paint_setup(3, bl, 0, height, RCT2_GLOBAL(0x00F4419C, uint32));

	bl = direction & 1 ? 7 : 8;
	metal_a_supports_paint_setup(3, bl, 0, height, RCT2_GLOBAL(0x00F4419C, uint32));

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D4, uint16) = 0xFFFF;

	uint32 eax = 0xFFFF0000;
	eax |= ((height & 0x0FFF) >> 4);
	eax |= (6 << 8);
	if (direction & 1) {
		RCT2_ADDRESS(0x009E30B6, uint32)[RCT2_GLOBAL(0x141F56B, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56B, uint8)++;
	}
	else {
		RCT2_ADDRESS(0x009E3138, uint32)[RCT2_GLOBAL(0x141F56A, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56A, uint8)++;
	}

	station_platform_begin_back_paint_setup(ride, direction, height, mapElement);

	station_platform_entrance_style_back_paint_setup(direction, height);

	station_platform_begin_front_paint_setup(direction, height, mapElement);

	// Note in vanilla this draw call was after underground check
	// so it would not be displayed when underground
	station_platform_begin_fence_front_paint_setup(direction, height);

	station_platform_entrance_style_front_paint_setup(direction, height);

	station_platform_begin_fence_back_paint_setup(direction, height);

	height += 25;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x00141E9DA, uint8) = 32;
	}
}

/* rct2: 0x005151B9 */
void junior_rc_middle_station_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	rct_ride* ride = get_ride(rideIndex);

	uint8 entrance_style = ride->entrance_style;

	uint32 image_id = RideEntranceDefinitions[entrance_style].station_image_id;
	if (!(RCT2_GLOBAL(0x00F441A0, uint32) & (1 << 29))) {
		image_id &= 0x7FFFF;
	}

	RCT2_GLOBAL(0x00F441E8, uint32) = image_id;
	RCT2_GLOBAL(0x00F441E4, uint32) = image_id;

	image_id = RCT2_GLOBAL(0x00F441A0, uint32);
	image_id |= 22428;
	image_id += direction & 1;

	//di
	uint8 lengthX = direction & 1 ? 28 : 32;
	//si
	uint8 lengthY = direction & 1 ? 32 : 28;

	sub_98197C(image_id, 0, 0, lengthX, lengthY, 1, height - 2, direction & 1 ? 2 : 0, direction & 1 ? 0 : 2, height, get_current_rotation());

	image_id = RCT2_GLOBAL(0x00F44198, uint32);
	image_id |= 28189;
	image_id += direction & 1;

	lengthX = direction & 1 ? 20 : 32;
	lengthY = direction & 1 ? 32 : 20;
	sint8 offsetX = direction & 1 ? 6 : 0;
	sint8 offsetY = direction & 1 ? 0 : 6;

	sub_98199C(image_id, offsetX, offsetY, lengthX, lengthY, 1, height, 0, 0, height, get_current_rotation());

	uint8 bl = direction & 1 ? 6 : 5;
	metal_a_supports_paint_setup(3, bl, 0, height, RCT2_GLOBAL(0x00F4419C, uint32));

	bl = direction & 1 ? 7 : 8;
	metal_a_supports_paint_setup(3, bl, 0, height, RCT2_GLOBAL(0x00F4419C, uint32));

	RCT2_GLOBAL(0x141E9B4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9B8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9BC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C4, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9C8, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9CC, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D0, uint16) = 0xFFFF;
	RCT2_GLOBAL(0x141E9D4, uint16) = 0xFFFF;

	uint32 eax = 0xFFFF0000;
	eax |= ((height & 0x0FFF) >> 4);
	eax |= (6 << 8);
	if (direction & 1) {
		RCT2_ADDRESS(0x009E30B6, uint32)[RCT2_GLOBAL(0x141F56B, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56B, uint8)++;
	}
	else {
		RCT2_ADDRESS(0x009E3138, uint32)[RCT2_GLOBAL(0x141F56A, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56A, uint8)++;
	}

	station_platform_back_paint_setup(ride, direction, height, mapElement, false);
	//0x0515791

	station_platform_entrance_style_back_paint_setup(direction, height);

	station_platform_front_paint_setup(direction, height, mapElement, false);

	// Note in vanilla this draw call was after underground check
	// so it would not be displayed when underground
	station_platform_fence_front_paint_setup(direction, height, false);

	station_platform_entrance_style_front_paint_setup(direction, height);

	station_platform_fence_back_paint_setup(direction, height, false);

	height += 25;
	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height;
		RCT2_GLOBAL(0x00141E9DA, uint8) = 32;
	}
}

/* rct2: 0x0051881E */
void junior_rc_25_deg_up_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {

	uint32 image_id = RCT2_GLOBAL(0x00F44198, uint32);
	switch (direction) {
	case 0:
		image_id |= SPR_JUNIOR_RC_25_DEG_NE_SW;
		break;
	case 1:
		image_id |= SPR_JUNIOR_RC_25_DEG_NW_SE;
		break;
	case 2:
		image_id |= SPR_JUNIOR_RC_25_DEG_SW_NE;
		break;
	case 3:
		image_id |= SPR_JUNIOR_RC_25_DEG_SE_NW;
		break;
	}
	
	if (mapElement->type & (1 << 7)) {
		image_id += 106;
	}
	//al
	sint8 offsetX = direction & 1 ? 6 : 0;
	//cl
	sint8 offsetY = direction & 1 ? 0 : 6;
	//di
	uint8 lengthX = direction & 1 ? 20 : 32;
	//si
	uint8 lengthY = direction & 1 ? 32 : 20;

	sub_98196C(image_id, offsetX, offsetY, lengthX, lengthY, 1, height, get_current_rotation());

	if (junior_rc_support_should_place()) {
		int edi = direction & 1 ? 2 : 1;
		metal_a_supports_paint_setup(edi, 4, 8, height, RCT2_GLOBAL(0x00F4419C, uint32));
	}

	RCT2_GLOBAL(0x00141E9C4, uint16) = 0xFFFF;
	uint32 eax = 0xFFFF0000;

	if (direction & 1) {
		eax |= 0x200 | ((height + 8) >> 4);
		RCT2_GLOBAL(0x00141E9D4, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9C8, uint16) = 0xFFFF;

		RCT2_ADDRESS(0x009E30B6, uint32)[RCT2_GLOBAL(0x141F56B, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56B, uint8)++;
	}
	else {
		eax |= 0x100 | ((height - 8) >> 4);
		RCT2_GLOBAL(0x00141E9D0, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9CC, uint16) = 0xFFFF;

		RCT2_ADDRESS(0x009E3138, uint32)[RCT2_GLOBAL(0x141F56A, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56A, uint8)++;
	}

	height += 56;
	if (RCT2_GLOBAL(0x141E9D8, sint16) < height) {
		RCT2_GLOBAL(0x141E9D8, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/* rct2: 0x00518B42 */
void junior_rc_flat_to_25_deg_up_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {

	uint32 image_id = RCT2_GLOBAL(0x00F44198, uint32);
	switch (direction) {
	case 0:
		image_id |= SPR_JUNIOR_RC_FLAT_TO_25_DEG_UP_NE_SW;
		break;
	case 1:
		image_id |= SPR_JUNIOR_RC_FLAT_TO_25_DEG_UP_NW_SE;
		break;
	case 2:
		image_id |= SPR_JUNIOR_RC_FLAT_TO_25_DEG_UP_SW_NE;
		break;
	case 3:
		image_id |= SPR_JUNIOR_RC_FLAT_TO_25_DEG_UP_SE_NW;
		break;
	}

	if (mapElement->type & (1 << 7)) {
		image_id += 106;
	}
	//al
	sint8 offsetX = direction & 1 ? 6 : 0;
	//cl
	sint8 offsetY = direction & 1 ? 0 : 6;
	//di
	uint8 lengthX = direction & 1 ? 20 : 32;
	//si
	uint8 lengthY = direction & 1 ? 32 : 20;

	sub_98196C(image_id, offsetX, offsetY, lengthX, lengthY, 1, height, get_current_rotation());

	if (junior_rc_support_should_place()) {
		int edi = direction & 1 ? 2 : 1;
		uint16 ax = (direction == 0) ? 5 : 3;
		metal_a_supports_paint_setup(edi, 4, ax, height, RCT2_GLOBAL(0x00F4419C, uint32));
	}

	RCT2_GLOBAL(0x00141E9C4, uint16) = 0xFFFF;
	uint32 eax = 0xFFFF0000;

	if (direction & 1) {
		eax |= 0x000 | (height >> 4);
		RCT2_GLOBAL(0x00141E9D4, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9C8, uint16) = 0xFFFF;

		RCT2_ADDRESS(0x009E30B6, uint32)[RCT2_GLOBAL(0x141F56B, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56B, uint8)++;
	}
	else {
		eax |= 0x000 | (height >> 4);
		RCT2_GLOBAL(0x00141E9D0, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9CC, uint16) = 0xFFFF;

		RCT2_ADDRESS(0x009E3138, uint32)[RCT2_GLOBAL(0x141F56A, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56A, uint8)++;
	}

	height += 48;
	if (RCT2_GLOBAL(0x141E9D8, sint16) < height) {
		RCT2_GLOBAL(0x141E9D8, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/* rct2: 0x00518E56 */
void junior_rc_25_deg_up_to_flat_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {

	uint32 image_id = RCT2_GLOBAL(0x00F44198, uint32);
	switch (direction) {
	case 0:
		image_id |= SPR_JUNIOR_RC_25_DEG_UP_TO_FLAT_NE_SW;
		break;
	case 1:
		image_id |= SPR_JUNIOR_RC_25_DEG_UP_TO_FLAT_NW_SE;
		break;
	case 2:
		image_id |= SPR_JUNIOR_RC_25_DEG_UP_TO_FLAT_SW_NE;
		break;
	case 3:
		image_id |= SPR_JUNIOR_RC_25_DEG_UP_TO_FLAT_SE_NW;
		break;
	}

	if (mapElement->type & (1 << 7)) {
		image_id += 106;
	}
	//al
	sint8 offsetX = direction & 1 ? 6 : 0;
	//cl
	sint8 offsetY = direction & 1 ? 0 : 6;
	//di
	uint8 lengthX = direction & 1 ? 20 : 32;
	//si
	uint8 lengthY = direction & 1 ? 32 : 20;

	sub_98196C(image_id, offsetX, offsetY, lengthX, lengthY, 1, height, get_current_rotation());

	if (junior_rc_support_should_place()) {
		int edi = direction & 1 ? 2 : 1;
		metal_a_supports_paint_setup(edi, 4, 6, height, RCT2_GLOBAL(0x00F4419C, uint32));
	}

	RCT2_GLOBAL(0x00141E9C4, uint16) = 0xFFFF;
	uint32 eax = 0xFFFF0000;
	if (direction == 1 || direction == 2) {
		eax |= 0xC00 | ((height + 8) >> 4);
	}
	else {
		eax |= 0x000 | ((height - 8) >> 4);
	}

	if (direction & 1) {
		RCT2_GLOBAL(0x00141E9D4, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9C8, uint16) = 0xFFFF;

		RCT2_ADDRESS(0x009E30B6, uint32)[RCT2_GLOBAL(0x141F56B, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56B, uint8)++;
	}
	else {
		RCT2_GLOBAL(0x00141E9D0, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9CC, uint16) = 0xFFFF;

		RCT2_ADDRESS(0x009E3138, uint32)[RCT2_GLOBAL(0x141F56A, uint8) / 2] = eax;
		RCT2_GLOBAL(0x141F56A, uint8)++;
	}

	height += 40;
	if (RCT2_GLOBAL(0x141E9D8, sint16) < height) {
		RCT2_GLOBAL(0x141E9D8, sint16) = height;
		RCT2_GLOBAL(0x141E9DA, uint8) = 32;
	}
}

/* rct2: 0x005189B0 */
void junior_rc_25_deg_down_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {

	junior_rc_25_deg_up_paint_setup(rideIndex, trackSequence, (direction + 2) & 3, height, mapElement);
}

/* rct2: 0x00518FE8 */
void junior_rc_flat_to_25_deg_down_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {

	junior_rc_25_deg_up_to_flat_paint_setup(rideIndex, trackSequence, (direction + 2) & 3, height, mapElement);
}

/* rct2: 0x00518CCC */
void junior_rc_25_deg_down_to_flat_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {

	junior_rc_flat_to_25_deg_up_paint_setup(rideIndex, trackSequence, (direction + 2) & 3, height, mapElement);
}

/* rct2: 0x0051917A */
void junior_rc_left_quarter_turn_5_tiles_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element* mapElement) {
	uint32 image_id;
	switch (trackSequence) {
	case 0:
		//51974e
		image_id = RCT2_GLOBAL(0x00F44198, uint32);
		image_id |= 27842;//27847 27852 27832
		sub_98196C(image_id, 0, 6, 32, 20, 1, height, get_current_rotation());
		metal_a_supports_paint_setup(1, 4, 0, height, RCT2_GLOBAL(0x00F4419C, uint32));

		RCT2_GLOBAL(0x00141E9C4, uint16) = 0xFFFF;
		

		uint32 eax = 0xFFFF0000;
		if (direction == 1 || direction == 2) {
			eax |= 0xC00 | (height >> 4);
		}
		else {
			eax |= 0x000 | (height >> 4);
		}

		if (direction & 1) {
			RCT2_GLOBAL(0x00141E9D4, uint16) = 0xFFFF;
			RCT2_GLOBAL(0x00141E9C8, uint16) = 0xFFFF;
			RCT2_GLOBAL(0x00141E9BC, uint16) = 0xFFFF;//Changes each rot

			//RCT2_ADDRESS(0x009E30B6, uint32)[RCT2_GLOBAL(0x141F56B, uint8) / 2] = eax;
			//RCT2_GLOBAL(0x141F56B, uint8)++; only rot 3
		}
		else {
			RCT2_GLOBAL(0x00141E9D0, uint16) = 0xFFFF;
			RCT2_GLOBAL(0x00141E9CC, uint16) = 0xFFFF;
			RCT2_GLOBAL(0x00141E9B4, uint16) = 0xFFFF;//Changes each rot

			RCT2_ADDRESS(0x009E3138, uint32)[RCT2_GLOBAL(0x141F56A, uint8) / 2] = eax;
			RCT2_GLOBAL(0x141F56A, uint8)++;
		}

		height += 32;
		if (RCT2_GLOBAL(0x141E9D8, sint16) < height) {
			RCT2_GLOBAL(0x141E9D8, sint16) = height;
			RCT2_GLOBAL(0x141E9DA, uint8) = 32;
		}
		break;
	case 1:
		//5196B7
		break;
	case 2:
		//5196D4
		break;
	case 3:
		//51964F
		break;
	case 4:
		//5195B8
		break;
	case 5:
		//5195D5
		break;
	case 6:
		//519534
		break;
	}
}

static void push_tunnel_left(uint16 height, uint8 type)
{
	uint32 eax = 0xFFFF0000 | ((height / 16) & 0xFF) | type << 8;
	RCT2_ADDRESS(0x009E3138, uint32)[RCT2_GLOBAL(0x141F56A, uint8) / 2] = eax;
	RCT2_GLOBAL(0x141F56A, uint8)++;
}

static void push_tunnel_right(uint16 height, uint8 type)
{
	uint32 eax = 0xFFFF0000 | ((height / 16) & 0xFF) | type << 8;
	RCT2_ADDRESS(0x009E30B6, uint32)[RCT2_GLOBAL(0x141F56B, uint8) / 2] = eax;
	RCT2_GLOBAL(0x141F56B, uint8)++;
}

/**
 * rct2: 0x008AAFC0, 0x00521593, 0x005216A1, 0x005217AF, 0x00521884
 */
void junior_rc_flat_to_left_bank_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element * mapElement)
{
	uint32 image_id;

	switch (direction) {
		case 0:
			image_id = SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_SW_NE | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 32, 20, 1, height, 0, 6, height, get_current_rotation());

			image_id = SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_SW_NE_FRONT | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 32, 1, 26, height, 0, 27, height, get_current_rotation());
			break;

		case 1:
			image_id = SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_NW_SE | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 20, 32, 1, height, 6, 0, height, get_current_rotation());

			image_id = SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_NW_SE_FRONT | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 1, 32, 26, height, 27, 0, height, get_current_rotation());
			break;

		case 2:
			image_id = SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_NE_SW | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 32, 20, 1, height, 0, 6, height, get_current_rotation());
			break;

		case 3:
			image_id = SPR_JUNIOR_RC_FLAT_TO_LEFT_BANK_SE_NW | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 20, 32, 1, height, 6, 0, height, get_current_rotation());
			break;
	}

	if (junior_rc_support_should_place()) {
		int edi = direction & 1 ? 2 : 1;
		metal_a_supports_paint_setup(edi, 4, 0, height, RCT2_GLOBAL(0x00F4419C, uint32));
	}

	if (direction & 1) {
		RCT2_GLOBAL(0x00141E9D0, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9C4, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9CC, uint16) = 0xFFFF;

		push_tunnel_left(height, 0);
	} else {
		RCT2_GLOBAL(0x00141E9D4, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9C4, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9C8, uint16) = 0xFFFF;

		push_tunnel_right(height, 0);
	}

	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height + 32) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height + 32;
		RCT2_GLOBAL(0x141E9DA, uint8) = 0x20;
	}
}

/**
 * rct2: 0x008AAFD0, 0x00521959, 0x00521A2E, 0x00521B03, 0x00521C11
 */
void junior_rc_flat_to_right_bank_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element * mapElement)
{
	uint32 image_id;

	switch (direction) {
		case 0:
			image_id = SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_SW_NE | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 32, 20, 1, height, 0, 6, height, get_current_rotation());
			break;

		case 1:
			image_id = SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_NW_SE | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 20, 32, 1, height, 6, 0, height, get_current_rotation());
			break;

		case 2:
			image_id = SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_NE_SW | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 32, 20, 1, height, 0, 6, height, get_current_rotation());

			image_id = SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_NE_SW_FRONT | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 32, 1, 26, height, 0, 27, height, get_current_rotation());
			break;

		case 3:
			image_id = SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_SE_NW | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 20, 32, 1, height, 6, 0, height, get_current_rotation());

			image_id = SPR_JUNIOR_RC_FLAT_TO_RIGHT_BANK_SE_NW_FRONT | RCT2_GLOBAL(0x00F44198, uint32);
			sub_98197C(image_id, 0, 0, 1, 32, 26, height, 27, 0, height, get_current_rotation());
			break;
	}

	if (junior_rc_support_should_place()) {
		int edi = direction & 1 ? 2 : 1;
		metal_a_supports_paint_setup(edi, 4, 0, height, RCT2_GLOBAL(0x00F4419C, uint32));
	}

	if (direction & 1) {
		RCT2_GLOBAL(0x00141E9D0, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9C4, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9CC, uint16) = 0xFFFF;

		push_tunnel_left(height, 0);
	} else {
		RCT2_GLOBAL(0x00141E9D4, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9C4, uint16) = 0xFFFF;
		RCT2_GLOBAL(0x00141E9C8, uint16) = 0xFFFF;

		push_tunnel_right(height, 0);
	}

	if (RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) < height + 32) {
		RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, sint16) = height + 32;
		RCT2_GLOBAL(0x141E9DA, uint8) = 0x20;
	}
}

void junior_rc_left_bank_to_flat_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element * mapElement)
{
	junior_rc_flat_to_right_bank_paint_setup(rideIndex, trackSequence, (direction + 2) % 4, height, mapElement);
}

void junior_rc_right_bank_to_flat_paint_setup(uint8 rideIndex, uint8 trackSequence, uint8 direction, int height, rct_map_element * mapElement)
{
	junior_rc_flat_to_left_bank_paint_setup(rideIndex, trackSequence, (direction + 2) % 4, height, mapElement);
}

/* 0x008AAA0C */
TRACK_PAINT_FUNCTION get_track_paint_function_junior_rc(int trackType, int direction) {
	switch (trackType) {
	case TRACK_ELEM_FLAT:
		return junior_rc_flat_paint_setup;
	case TRACK_ELEM_END_STATION:
		return junior_rc_end_station_paint_setup;
	case TRACK_ELEM_BEGIN_STATION:
		return junior_rc_begin_station_paint_setup;
	case TRACK_ELEM_MIDDLE_STATION:
		return junior_rc_middle_station_paint_setup;
	case TRACK_ELEM_25_DEG_UP:
		return junior_rc_25_deg_up_paint_setup;

	case TRACK_ELEM_FLAT_TO_25_DEG_UP:
		return junior_rc_flat_to_25_deg_up_paint_setup;

	case TRACK_ELEM_25_DEG_UP_TO_FLAT:
		return junior_rc_25_deg_up_to_flat_paint_setup;
	case TRACK_ELEM_25_DEG_DOWN:
		return junior_rc_25_deg_down_paint_setup;

	case TRACK_ELEM_FLAT_TO_25_DEG_DOWN:
		return junior_rc_flat_to_25_deg_down_paint_setup;

	case TRACK_ELEM_25_DEG_DOWN_TO_FLAT:
		return junior_rc_25_deg_down_to_flat_paint_setup;
	case TRACK_ELEM_LEFT_QUARTER_TURN_5_TILES:
		return junior_rc_left_quarter_turn_5_tiles_paint_setup;
	case TRACK_ELEM_RIGHT_QUARTER_TURN_5_TILES:
		return NULL;
	case TRACK_ELEM_FLAT_TO_LEFT_BANK:
		return junior_rc_flat_to_left_bank_paint_setup;
	case TRACK_ELEM_FLAT_TO_RIGHT_BANK:
		return junior_rc_flat_to_right_bank_paint_setup;
	case TRACK_ELEM_LEFT_BANK_TO_FLAT:
		return junior_rc_left_bank_to_flat_paint_setup;
	case TRACK_ELEM_RIGHT_BANK_TO_FLAT:
		return junior_rc_right_bank_to_flat_paint_setup;
	case TRACK_ELEM_BANKED_LEFT_QUARTER_TURN_5_TILES:
	case TRACK_ELEM_BANKED_RIGHT_QUARTER_TURN_5_TILES:
	case TRACK_ELEM_LEFT_BANK_TO_25_DEG_UP:
	case TRACK_ELEM_RIGHT_BANK_TO_25_DEG_UP:
	case TRACK_ELEM_25_DEG_UP_TO_LEFT_BANK:
	case TRACK_ELEM_25_DEG_UP_TO_RIGHT_BANK:
	case TRACK_ELEM_LEFT_BANK_TO_25_DEG_DOWN:
	case TRACK_ELEM_RIGHT_BANK_TO_25_DEG_DOWN:
	case TRACK_ELEM_25_DEG_DOWN_TO_LEFT_BANK:
	case TRACK_ELEM_25_DEG_DOWN_TO_RIGHT_BANK:
	case TRACK_ELEM_LEFT_BANK:
	case TRACK_ELEM_RIGHT_BANK:
	case TRACK_ELEM_LEFT_QUARTER_TURN_5_TILES_25_DEG_UP:
	case TRACK_ELEM_RIGHT_QUARTER_TURN_5_TILES_25_DEG_UP:
	case TRACK_ELEM_LEFT_QUARTER_TURN_5_TILES_25_DEG_DOWN:
	case TRACK_ELEM_RIGHT_QUARTER_TURN_5_TILES_25_DEG_DOWN:
	case TRACK_ELEM_S_BEND_LEFT:
	case TRACK_ELEM_S_BEND_RIGHT:
		return NULL;
	}
	return NULL;
}