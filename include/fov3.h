/*
* libtcod 1.5.0
* Copyright (c) 2008,2009 J.C.Wilk
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of J.C.Wilk may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY J.C.WILK ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL J.C.WILK BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

typedef void *TCOD_map3_t;

// FOV3_DIAMOND : Joe Osborn's modification of http://www.geocities.com/temerra/los_rays.html

typedef enum {
  // FOV_BASIC,
	FOV3_DIAMOND,
  // FOV_SHADOW,
  // FOV_PERMISSIVE_0,
  // FOV_PERMISSIVE_1,
  // FOV_PERMISSIVE_2,
  // FOV_PERMISSIVE_3,
  // FOV_PERMISSIVE_4,
  // FOV_PERMISSIVE_5,
  // FOV_PERMISSIVE_6,
  // FOV_PERMISSIVE_7,
  // FOV_PERMISSIVE_8,
  // FOV_RESTRICTIVE,
	NB_FOV3_ALGORITHMS } TCOD_fov3_algorithm_t;


TCODLIB_API TCOD_map3_t TCOD_map3_new(int width, int height, int depth);
TCODLIB_API void TCOD_map3_clear(TCOD_map3_t map);
// copy a map to another, reallocating it when needed
TCODLIB_API void TCOD_map3_copy(TCOD_map3_t source, TCOD_map3_t dest);
// change a cell properties
TCODLIB_API void TCOD_map3_set_properties(TCOD_map3_t map, int x, int y, int z, bool is_transparent, bool floor_transparent, bool ceiling_transparent, bool is_walkable, bool ramp_down, bool ramp_up);
TCODLIB_API void TCOD_map3_delete(TCOD_map3_t map);

TCODLIB_API void TCOD_map3_compute_fov(TCOD_map3_t map, int player_x, int player_y, int player_z, int max_radius, bool light_walls, TCOD_fov3_algorithm_t algo);
TCODLIB_API bool TCOD_map3_is_in_fov(TCOD_map3_t map, int x, int y, int z);
TCODLIB_API void TCOD_map3_set_in_fov(TCOD_map3_t map, int x, int y, int z, bool fov);
TCODLIB_API bool TCOD_map3_is_transparent(TCOD_map3_t map, int x, int y, int z);
TCODLIB_API bool TCOD_map3_is_ceiling_transparent(TCOD_map3_t map, int x, int y, int z);
TCODLIB_API bool TCOD_map3_is_floor_transparent(TCOD_map3_t map, int x, int y, int z);
TCODLIB_API bool TCOD_map3_is_walkable(TCOD_map3_t map, int x, int y, int z);
TCODLIB_API bool TCOD_map3_is_down_ramp(TCOD_map3_t map, int x, int y, int z);
TCODLIB_API bool TCOD_map3_is_up_ramp(TCOD_map3_t map, int x, int y, int z);
TCODLIB_API int TCOD_map3_get_width(TCOD_map3_t map);
TCODLIB_API int TCOD_map3_get_height(TCOD_map3_t map);
TCODLIB_API int TCOD_map3_get_depth(TCOD_map3_t map);
TCODLIB_API int TCOD_map3_get_nb_cells(TCOD_map3_t map);
