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

#include <stdlib.h>
#include "libtcod.h"
#include "libtcod_int.h"
#include <string.h>

typedef struct _ray3_data_t {
	int xloc,yloc,zloc; // position
	int xob,yob,zob; // obscurity vector
	int xerr,yerr,zerr; // bresenham error
	struct _ray3_data_t * xinput, * yinput, * zinput; // offset of input rays
	bool added; // already in the fov
	bool ignore; // non visible. don't bother processing it
} ray3_data_t;

static int origx,origy,origz; // fov origin
static ray3_data_t **raymap; // result rays
static ray3_data_t *raymap2; // temporary rays
static int perimidx;

static ray3_data_t *new_ray(map3_t *m,int x, int y, int z) {
    ray3_data_t *r;
	if ( (unsigned) (x+origx) >= (unsigned)m->width ) return NULL;
	if ( (unsigned) (y+origy) >= (unsigned)m->height ) return NULL;
	if ( (unsigned) (z+origz) >= (unsigned)m->depth ) return NULL;
  int ind = MAP_COORD3(m,x+origx,y+origy,z+origz);
	r=&raymap2[ind];
	r->xloc=x;
	r->yloc=y;
  r->zloc=z;
	return r;
}

static void processRay(map3_t *m, TCOD_list_t perim, ray3_data_t *new_ray, ray3_data_t *input_ray) {
	if ( new_ray ) {
		int mapx=origx+new_ray->xloc;
		int mapy=origy+new_ray->yloc;
		int mapz=origz+new_ray->zloc;
		int newrayidx;
		newrayidx=MAP_COORD3(m, mapx, mapy, mapz);
		if(new_ray->zloc == input_ray->zloc) {
		  if(new_ray->yloc == input_ray->yloc) {
        new_ray->xinput = input_ray;
		  } else if(new_ray->xloc == input_ray->xloc) {
        new_ray->yinput = input_ray;
		  }
		} else {
      new_ray->zinput = input_ray;
		}
		//don't know if the above is correct

		if (! new_ray->added) {
			TCOD_list_push(perim,new_ray);
			new_ray->added=true;
			raymap[newrayidx] = new_ray;
		}
	}
}

#define IS_OBSCURE(r) (\
  (r->xerr > 0 && r->xerr <= r->xob) || \
  (r->yerr > 0 && r->yerr <= r->yob) || \
  (r->zerr > 0 && r->zerr <= r->zob) \
)

static void process_x_input(ray3_data_t *new_ray, ray3_data_t *xinput) {
	if ( xinput->xob == 0 && xinput->yob == 0 && xinput->zob == 0) return;
	if ( xinput->xerr > 0 && new_ray->xob == 0) {
		new_ray->xerr = xinput->xerr - xinput->yob - xinput->zob;
		new_ray->yerr = xinput->yerr + xinput->yob;
		new_ray->zerr = xinput->zerr + xinput->zob;
		new_ray->xob=xinput->xob;
		new_ray->yob=xinput->yob;
	}
	if ( xinput->yerr <= 0 && xinput->yob > 0 && xinput->xerr > 0) {
		new_ray->yerr = xinput->yerr + xinput->yob;
		new_ray->xerr = xinput->xerr - xinput->yob;
		new_ray->xob=xinput->xob;
		new_ray->yob=xinput->yob;
	}
	if ( xinput->zerr <= 0 && xinput->zob > 0 && xinput->xerr > 0) {
		new_ray->zerr = xinput->zerr + xinput->zob;
		new_ray->xerr = xinput->xerr - xinput->zob;
		new_ray->xob=xinput->xob;
		new_ray->zob=xinput->zob;
	}
}

static void process_y_input(ray3_data_t *new_ray, ray3_data_t *yinput) {
	if ( yinput->xob == 0 && yinput->yob == 0 && yinput->zob == 0 ) return;
	if ( yinput->yerr > 0 && new_ray->yob == 0) {
		new_ray->yerr = yinput->yerr - yinput->xob - yinput->zob;
		new_ray->zerr = yinput->zerr + yinput->zob;
		new_ray->xerr = yinput->xerr + yinput->xob;
		new_ray->xob=yinput->xob;
		new_ray->yob=yinput->yob;
	}
	if ( yinput->xerr <= 0 && yinput->xob > 0 && yinput->yerr > 0) {
		new_ray->yerr = yinput->yerr - yinput->xob;
		new_ray->xerr = yinput->xerr + yinput->xob;
		new_ray->xob=yinput->xob;
		new_ray->yob=yinput->yob;
	}
	if ( yinput->zerr <= 0 && yinput->zob > 0 && yinput->yerr > 0) {
		new_ray->yerr = yinput->yerr - yinput->zob;
		new_ray->zerr = yinput->zerr + yinput->zob;
		new_ray->zob=yinput->zob;
		new_ray->yob=yinput->yob;
	}
}

static void process_z_input(ray3_data_t *new_ray, ray3_data_t *zinput) {
	if ( zinput->xob == 0 && zinput->yob == 0 && zinput->zob == 0 ) return;
	if ( zinput->zerr > 0 && new_ray->zob == 0) {
		new_ray->zerr = zinput->zerr - zinput->xob - zinput->yob;
		new_ray->yerr = zinput->yerr + zinput->yob;
		new_ray->xerr = zinput->xerr + zinput->xob;
		new_ray->xob=zinput->xob;
		new_ray->yob=zinput->yob;
		new_ray->zob=zinput->zob;
	}
	if ( zinput->xerr <= 0 && zinput->xob > 0 && zinput->zerr > 0) {
		new_ray->zerr = zinput->zerr - zinput->xob;
		new_ray->xerr = zinput->xerr + zinput->xob;
		new_ray->xob=zinput->xob;
		new_ray->zob=zinput->zob;
	}
	if ( zinput->yerr <= 0 && zinput->yob > 0 && zinput->zerr > 0) {
		new_ray->zerr = zinput->zerr - zinput->xob;
		new_ray->yerr = zinput->yerr + zinput->yob;
		new_ray->yob=zinput->yob;
		new_ray->zob=zinput->zob;
	}
}


static void merge_input(map3_t *m, ray3_data_t *r) {
	int rayidx=MAP_COORD3(m, r->xloc+origx, r->yloc+origy, r->zloc+origz);
	ray3_data_t *xi=r->xinput;
	ray3_data_t *yi=r->yinput;
	ray3_data_t *zi=r->zinput;
	if ( xi ) process_x_input(r,xi);
	if ( yi ) process_y_input(r,yi);
	if ( zi ) process_z_input(r,zi);
	if ( ! xi ) {
	  if(yi && zi) {
  		if ( IS_OBSCURE(yi) && IS_OBSCURE(zi) ) r->ignore=true;
	  } else if (zi) {
	    if ( IS_OBSCURE(zi) ) r->ignore=true;
	  } else {
	    if ( IS_OBSCURE(yi) ) r->ignore=true;
	  }
	} else if ( ! yi ) {
	  if(zi && xi) {
  		if ( IS_OBSCURE(xi) && IS_OBSCURE(zi) ) r->ignore=true;
	  } else if (zi) {
	    if ( IS_OBSCURE(zi) ) r->ignore=true;
	  } else {
	    if ( IS_OBSCURE(xi) ) r->ignore=true;
	  }
	} else if ( ! zi ) {
	  if(yi && xi) {
  		if ( IS_OBSCURE(yi) && IS_OBSCURE(xi) ) r->ignore=true;
	  } else if(yi) {
	    if ( IS_OBSCURE(yi) ) r->ignore=true;
	  } else {
	    if ( IS_OBSCURE(xi) ) r->ignore=true;
	  }
	} else if( IS_OBSCURE(xi) && IS_OBSCURE(yi) && IS_OBSCURE(zi) ) {
		r->ignore=true;
	}
  bool prop = false;
	if(r->zinput) {
  	int previdx=MAP_COORD3(m, r->zinput->xloc+origx, r->zinput->yloc+origy, r->zinput->zloc+origz);
	  if(r->zloc < r->zinput->zloc) {
	    //dz == -
      prop = (! r->ignore && !(m->cells[rayidx].ceilingTransparent && m->cells[previdx].floorTransparent));
	  } else {
	    //dz == +
      prop = (! r->ignore && !(m->cells[rayidx].floorTransparent && m->cells[previdx].ceilingTransparent));
	  }
	} else {
    prop = (! r->ignore && !m->cells[rayidx].transparent);
	}
	if(prop) {
  	r->xerr = r->xob = ABS(r->xloc);
		r->yerr = r->yob = ABS(r->yloc);
		r->zerr = r->zob = ABS(r->zloc);
	}
}

static void expandPerimeterFrom(map3_t *m,TCOD_list_t perim,ray3_data_t *r) {
	if ( r->xloc >= 0 ) {
		processRay(m,perim,new_ray(m,r->xloc+1,r->yloc,r->zloc),r);
	}
	if ( r->xloc <= 0 ) {
		processRay(m,perim,new_ray(m,r->xloc-1,r->yloc,r->zloc),r);
	}
	if ( r->yloc >= 0 ) {
		processRay(m,perim,new_ray(m,r->xloc,r->yloc+1,r->zloc),r);
	}
	if ( r->yloc <= 0 ) {
		processRay(m,perim,new_ray(m,r->xloc,r->yloc-1,r->zloc),r);
	}
	if ( r->zloc >= 0 ) {
		processRay(m,perim,new_ray(m,r->xloc,r->yloc,r->zloc+1),r);
	}
	if ( r->zloc <= 0 ) {
		processRay(m,perim,new_ray(m,r->xloc,r->yloc,r->zloc-1),r);
	}
}


void TCOD_map3_compute_fov_diamond_raycasting(TCOD_map3_t map, int player_x, int player_y, int player_z, int max_radius, bool light_walls) {
	map3_t *m = (map3_t *)map;
	TCOD_list_t perim=TCOD_list_allocate(m->nbcells);
	cell3_t *c;
	ray3_data_t **r;
	int nbcells;
	int r2=max_radius*max_radius;

	perimidx=0;
	raymap=(ray3_data_t **)calloc(sizeof(ray3_data_t*),m->nbcells);
	raymap2=(ray3_data_t *)calloc(sizeof(ray3_data_t),m->nbcells);
	origx=player_x;
	origy=player_y;
	origz=player_z;
	expandPerimeterFrom(m,perim,new_ray(m,0,0,0));
	while ( perimidx < TCOD_list_size(perim) ) {
		ray3_data_t *ray=(ray3_data_t *)TCOD_list_get(perim,perimidx);
		int distance = 0;
		if ( r2 > 0 ) distance = ((ray->xloc * ray->xloc) + (ray->yloc * ray->yloc) + (ray->zloc * ray->zloc));
		perimidx++;
		if ( distance <= r2) {
			merge_input(m, ray);
			if ( !ray->ignore ) expandPerimeterFrom(m,perim,ray);
		} else ray->ignore=true;
	}

	// set fov data
	c=m->cells;
	r=raymap;
	nbcells=m->nbcells;
	while ( nbcells!= 0 ) {
		if ( *r == NULL || (*r)->ignore
			|| ((*r)->xerr > 0 && (*r)->xerr <= (*r)->xob )
			|| ((*r)->yerr > 0 && (*r)->yerr <= (*r)->yob )
			|| ((*r)->zerr > 0 && (*r)->zerr <= (*r)->zob )
		) {
			c->fov=0;
		} else {
			c->fov=1;
		}
		c++;
		r++;
		nbcells--;
	}
	m->cells[MAP_COORD3(m,origx,origy,origz)].fov=1;

	// light walls
	if ( light_walls ) {
		int xmin=0, ymin=0, zmin=0, xmax=m->width, ymax=m->height, zmax=m->depth;
		if ( max_radius > 0 ) {
			xmin=MAX(0,player_x-max_radius);
			ymin=MAX(0,player_y-max_radius);
			zmin=MAX(0,player_z-max_radius);
			xmax=MIN(m->width,player_x+max_radius+1);
			ymax=MIN(m->height,player_y+max_radius+1);
			zmax=MIN(m->depth,player_z+max_radius+1);
		}
		//is this stuff right?
		TCOD_map3_postproc(m,xmin,ymin,zmin,player_x,player_y,player_z,-1,-1,-1);
		TCOD_map3_postproc(m,player_x,ymin,zmin,xmax-1,player_y,player_z,1,-1,-1);
		TCOD_map3_postproc(m,xmin,player_y,zmin,player_x,ymax-1,player_z,-1,1,-1);
		TCOD_map3_postproc(m,xmin,ymin,player_z,player_x,player_y,zmin-1,-1,-1,1);
		TCOD_map3_postproc(m,player_x,player_y,zmin,xmax-1,ymax-1,player_z,1,1,-1);
		TCOD_map3_postproc(m,xmin,player_y,player_z,player_x,ymax-1,zmax-1,-1,1,1);
		TCOD_map3_postproc(m,player_x,ymin,player_z,xmax-1,player_y,zmax-1,1,-1,1);
		TCOD_map3_postproc(m,player_x,player_y,player_z,xmax-1,ymax-1,zmax-1,1,1,1);
	}

	free(raymap);
	free(raymap2);
	TCOD_list_delete(perim);
}

void TCOD_map3_postproc(map3_t *map,int x0,int y0,int z0, int x1, int y1, int z1, int dx, int dy, int dz) {
	int cx,cy,cz;
	for (cx=x0; cx <= x1; cx++) {
		for (cy=y0;cy <= y1; cy ++ ) {
  		for (cz=z0;cz <= z1; cz ++ ) {
			  int x2 = cx+dx;
			  int y2 = cy+dy;
			  int z2 = cz+dz;
			  unsigned int offset=MAP_COORD3(map,cx,cy,cz);
			  //wrong, consider dz here
        bool sightCheck = false;
        bool walkCheck = false;
        bool walkCheck2 = false;
        if(dz == 0) {
          sightCheck = map->cells[offset].transparent;
          walkCheck = map->cells[offset].walkable;
        } else if(dz > 0) {
          sightCheck = map->cells[offset].floorTransparent;
          walkCheck = map->cells[offset].rampDown;
        } else {
          sightCheck = map->cells[offset].ceilingTransparent;
          walkCheck = map->cells[offset].rampUp;
        }
			  if ( offset < (unsigned)map->nbcells && map->cells[offset].fov == 1 && sightCheck && walkCheck) {
			  	if ( x2 >= x0 && x2 <= x1 ) {
			  		unsigned int offset2=MAP_COORD3(map,x2,cy,cz);
			  		if(dz == 0) {
              walkCheck2 = map->cells[offset2].walkable;
            } else if(dz > 0) {
              walkCheck2 = map->cells[offset2].rampUp;
            } else {
              walkCheck2 = map->cells[offset2].rampDown;
            }
            
			  		if ( offset2 < (unsigned)map->nbcells && ! walkCheck2 )
			  			map->cells[offset2].fov=1;
			  	}
			  	if ( y2 >= y0 && y2 <= y1 ) {
            unsigned int offset2=MAP_COORD3(map,cx,y2,cz);
			  		if(dz == 0) {
              walkCheck2 = map->cells[offset2].walkable;
            } else if(dz > 0) {
              walkCheck2 = map->cells[offset2].rampUp;
            } else {
              walkCheck2 = map->cells[offset2].rampDown;
            }

			  		if ( offset2 < (unsigned)map->nbcells && ! walkCheck2 )
			  			map->cells[offset2].fov=1;
			  	}
			  	if ( z2 >= z0 && z2 <= z1 ) {
            unsigned int offset2=MAP_COORD3(map,cx,cy,z2);
			  		if(dz == 0) {
              walkCheck2 = map->cells[offset2].walkable;
            } else if(dz > 0) {
              walkCheck2 = map->cells[offset2].rampUp;
            } else {
              walkCheck2 = map->cells[offset2].rampDown;
            }

			  		if ( offset2 < (unsigned)map->nbcells && ! walkCheck2 )
			  			map->cells[offset2].fov=1;
			  	}
			  	if ( x2 >= x0 && x2 <= x1 && y2 >= y0 && y2 <= y1 ) {
			  		unsigned int offset2=MAP_COORD3(map,x2,y2,cz);
			  		if(dz == 0) {
              walkCheck2 = map->cells[offset2].walkable;
            } else if(dz > 0) {
              walkCheck2 = map->cells[offset2].rampUp;
            } else {
              walkCheck2 = map->cells[offset2].rampDown;
            }

			  		if ( offset2 < (unsigned)map->nbcells && ! walkCheck2 )
			  			map->cells[offset2].fov=1;
			  	}
			  	//not sure this is correct.
			  	if ( x2 >= x0 && x2 <= x1 && z2 >= z0 && z2 <= z1 ) {
			  		unsigned int offset2=MAP_COORD3(map,x2,cy,z2);
			  		if(dz == 0) {
              walkCheck2 = map->cells[offset2].walkable;
            } else if(dz > 0) {
              walkCheck2 = map->cells[offset2].rampUp;
            } else {
              walkCheck2 = map->cells[offset2].rampDown;
            }

			  		if ( offset2 < (unsigned)map->nbcells && ! walkCheck2 )
			  			map->cells[offset2].fov=1;
			  	}
			  	if ( y2 >= y0 && y2 <= y1 && z2 >= z0 && z2 <= z1 ) {
			  		unsigned int offset2=MAP_COORD3(map,cx,y2,z2);
			  		if(dz == 0) {
              walkCheck2 = map->cells[offset2].walkable;
            } else if(dz > 0) {
              walkCheck2 = map->cells[offset2].rampUp;
            } else {
              walkCheck2 = map->cells[offset2].rampDown;
            }

			  		if ( offset2 < (unsigned)map->nbcells && ! walkCheck2 )
			  			map->cells[offset2].fov=1;
			  	}
			  	if ( x2 >= x0 && x2 <= x1 && y2 >= y0 && y2 <= y1 && z2 >= z0 && z2 <= z1 ) {
			  		unsigned int offset2=MAP_COORD3(map,x2,y2,z2);
			  		if(dz == 0) {
              walkCheck2 = map->cells[offset2].walkable;
            } else if(dz > 0) {
              walkCheck2 = map->cells[offset2].rampUp;
            } else {
              walkCheck2 = map->cells[offset2].rampDown;
            }

			  		if ( offset2 < (unsigned)map->nbcells && ! walkCheck2 )
			  			map->cells[offset2].fov=1;
			  	}
			  }
			}
		}
	}
}
