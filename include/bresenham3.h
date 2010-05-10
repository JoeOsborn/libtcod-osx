/*
* libtcod 1.5.0
* Copyright (c) 2008,2009 Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Jice ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Jice BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _TCOD_BRESENHAM3_H
#define _TCOD_BRESENHAM3_H

typedef bool (*TCOD_line3_listener_t) (int x, int y, int z);

TCODLIB_API void TCOD_line3_init(int xFrom, int yFrom, int zFrom, int xTo, int yTo, int zTo);
TCODLIB_API bool TCOD_line3_step(int *xCur, int *yCur, int *zCur); // advance one step. returns true if we reach destination
// atomic callback function. Stops when the callback returns false
TCODLIB_API bool TCOD_line3(int xFrom, int yFrom, int zFrom, int xTo, int yTo, int zTo, TCOD_line3_listener_t listener);

// thread-safe versions
typedef struct {
	int stepx;
	int stepy;
	int stepz;
	int e1;
	int e2;
	int deltax;
	int deltay;
	int deltaz;
	int origx; 
	int origy; 
	int origz; 
	int destx; 
	int desty; 
	int destz; 
} TCOD_bresenham3_data_t;

TCODLIB_API void TCOD_line3_init_mt(int xFrom, int yFrom, int zFrom, int xTo, int yTo, int zTo, TCOD_bresenham3_data_t *data);
TCODLIB_API bool TCOD_line3_step_mt(int *xCur, int *yCur, int *zCur, TCOD_bresenham3_data_t *data);
TCODLIB_API bool TCOD_line3_mt(int xFrom, int yFrom, int zFrom, int xTo, int yTo, int zTo, TCOD_line3_listener_t listener, TCOD_bresenham3_data_t *data);

#endif
