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

// class TCODPath3;

class TCODLIB_API TCODMap3 {
	public :
		TCODMap3(int width, int height, int depth);

    // TCODImage *toImage() const;
    // void fromImage(const TCODImage *img, const TCODColor &wallColor, const TCODColor &groundColor);
		
    // TCODHeightMap *toHeightmap() const;
    // void fromHeightmap(const TCODHeightMap *hm, float wallValue, float groundValue);
		
		void clear();
		void setProperties(int x,int y,int z, bool isTransparent, bool floorTransparent, bool ceilingTransparent, bool isWalkable, bool downRamp, bool upRamp);
		void copy (const TCODMap3 *source);
		virtual ~TCODMap3();
		void computeFov(int playerX,int playerY,int playerZ, int maxRadius = 0,bool light_walls = true, TCOD_fov3_algorithm_t algo = FOV3_DIAMOND);
		bool isInFov(int x,int y,int z) const;
		void setInFov(int x,int y,int z, bool fov);
		bool isTransparent(int x, int y, int z) const;
		bool isFloorTransparent(int x, int y, int z) const;
		bool isCeilingTransparent(int x, int y, int z) const;
		bool isWalkable(int x, int y, int z) const;
		bool isDownRamp(int x, int y, int z) const;
		bool isUpRamp(int x, int y, int z) const;
		int getWidth() const;
		int getHeight() const;
		int getDepth() const;
		int getNbCells() const;
    // friend class TCODLIB_API TCODPath3;
//	protected :
		TCOD_map3_t data;
};
