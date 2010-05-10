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
#include "libtcod.hpp"
TCODMap3::TCODMap3(int width,int height,int depth) {
	data = TCOD_map3_new(width,height,depth);
}

void TCODMap3::clear() {
	TCOD_map3_clear(data);
}

void TCODMap3::setProperties(int x, int y, int z, bool isTransparent, bool floorTransparent, bool ceilingTransparent, bool isWalkable, bool rampDown, bool rampUp) {
	TCOD_map3_set_properties(data,x,y,z,isTransparent,floorTransparent,ceilingTransparent,isWalkable,rampDown,rampUp);
}

void TCODMap3::copy(const TCODMap3 *source) {
	TCOD_map3_copy(source->data,data);
}

void TCODMap3::computeFov(int x, int y, int z, int maxRadius, bool light_walls, TCOD_fov3_algorithm_t algo) {
	TCOD_map3_compute_fov(data,x,y,z,maxRadius,light_walls, algo);
}

bool TCODMap3::isInFov(int x, int y, int z) const {
	return TCOD_map3_is_in_fov(data,x,y,z) != 0;
}

void TCODMap3::setInFov(int x,int y, int z, bool fov) {
	TCOD_map3_set_in_fov(data, x, y, z, fov);
}

bool TCODMap3::isTransparent(int x, int y, int z) const {
	return TCOD_map3_is_transparent(data,x,y,z) != 0;
}
bool TCODMap3::isFloorTransparent(int x, int y, int z) const {
	return TCOD_map3_is_floor_transparent(data,x,y,z) != 0;
}
bool TCODMap3::isCeilingTransparent(int x, int y, int z) const {
	return TCOD_map3_is_ceiling_transparent(data,x,y,z) != 0;
}


bool TCODMap3::isWalkable(int x, int y, int z) const {
	return TCOD_map3_is_walkable(data,x,y,z) != 0;
}
bool TCODMap3::isDownRamp(int x, int y, int z) const {
	return TCOD_map3_is_down_ramp(data,x,y,z) != 0;
}
bool TCODMap3::isUpRamp(int x, int y, int z) const {
	return TCOD_map3_is_up_ramp(data,x,y,z) != 0;
}


int TCODMap3::getWidth() const {
	return TCOD_map3_get_width(data);
}

int TCODMap3::getHeight() const {
	return TCOD_map3_get_height(data);
}

int TCODMap3::getDepth() const {
	return TCOD_map3_get_depth(data);
}


int TCODMap3::getNbCells() const {
	return TCOD_map3_get_nb_cells(data);
}

TCODMap3::~TCODMap3() {
	TCOD_map3_delete(data);
}
