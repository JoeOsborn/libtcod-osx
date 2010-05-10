#include <stdlib.h>
#include "libtcod.h"
#include "libtcod_int.h"
#include <string.h>

TCOD_map3_t TCOD_map3_new(int width, int height, int depth) {
  map3_t *map=NULL;
	TCOD_IFNOT(width > 0 && height > 0 && depth > 0) return NULL;
	map=(map3_t *)calloc(sizeof(map3_t),1);
	map->width=width;
	map->height=height;
  map->depth=depth;
	map->nbcells=width*height*depth;
	map->cells=(cell3_t *)calloc(sizeof(cell3_t),map->nbcells);
	return map;
}

void TCOD_map3_copy(TCOD_map3_t source, TCOD_map3_t dest) {
	map3_t *source_int = (map3_t *)source;
	map3_t *dest_int = (map3_t *)dest;
	TCOD_IFNOT(source != NULL && dest != NULL ) return;
	if ( dest_int->nbcells != source_int->nbcells ) {
		free(dest_int->cells);
		dest_int->cells=(cell3_t *)malloc(sizeof(cell3_t)*dest_int->nbcells);
	}
	dest_int->width=source_int->width;
	dest_int->height=source_int->height;
	dest_int->depth=source_int->depth;
	dest_int->nbcells=source_int->nbcells;
	memcpy(dest_int->cells, source_int->cells, sizeof(cell3_t) * source_int->nbcells);
}

void TCOD_map3_clear(TCOD_map3_t map) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return;
	memset(m->cells,0,sizeof(cell3_t)*m->width*m->height*m->depth);
}

void TCOD_map3_set_properties(TCOD_map3_t map, int x, int y, int z, bool is_transparent, bool floor_transparent, bool ceiling_transparent, bool is_walkable, bool ramp_down, bool ramp_up) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return;
	TCOD_IFNOT((unsigned)x <(unsigned)m->width && (unsigned)y < (unsigned)m->height && (unsigned)z < (unsigned)m->depth) return;
  int coord = MAP_COORD3(m,x,y,z);
	m->cells[coord].transparent=is_transparent;
	m->cells[coord].floorTransparent=floor_transparent;
	m->cells[coord].ceilingTransparent=ceiling_transparent;
	m->cells[coord].walkable=is_walkable;
	m->cells[coord].rampDown=ramp_down;
	m->cells[coord].rampUp=ramp_up;
}

void TCOD_map3_delete(TCOD_map3_t map) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return;
	free(m->cells);
	free(m);
}

void TCOD_map3_compute_fov(TCOD_map3_t map, int player_x, int player_y, int player_z, int max_radius, bool light_walls, TCOD_fov3_algorithm_t algo) {
	TCOD_IFNOT(map != NULL) return;
	switch(algo) {
		case FOV3_DIAMOND : TCOD_map3_compute_fov_diamond_raycasting(map,player_x,player_y,player_z,max_radius,light_walls);
		break;
		default:break;
	}
}

bool TCOD_map3_is_in_fov(TCOD_map3_t map, int x, int y, int z) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return false;
	TCOD_IFNOT((unsigned)x <(unsigned)m->width && (unsigned)y < (unsigned)m->height && (unsigned)z < (unsigned)m->depth) return false;
	return m->cells[MAP_COORD3(m,x,y,z)].fov == 1;
}

void TCOD_map3_set_in_fov(TCOD_map3_t map, int x, int y, int z, bool fov) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return;
	TCOD_IFNOT((unsigned)x <(unsigned)m->width && (unsigned)y < (unsigned)m->height && (unsigned)z < (unsigned)m->depth) return;
	m->cells[MAP_COORD3(m,x,y,z)].fov = fov ? 1:0;
}

bool TCOD_map3_is_transparent(TCOD_map3_t map, int x, int y, int z) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return false;
	TCOD_IFNOT((unsigned)x <(unsigned)m->width && (unsigned)y < (unsigned)m->height && (unsigned)z < (unsigned)m->depth) return false;
	return m->cells[MAP_COORD3(m,x,y,z)].transparent;
}
bool TCOD_map3_is_ceiling_transparent(TCOD_map3_t map, int x, int y, int z) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return false;
	TCOD_IFNOT((unsigned)x <(unsigned)m->width && (unsigned)y < (unsigned)m->height && (unsigned)z < (unsigned)m->depth) return false;
	return m->cells[MAP_COORD3(m,x,y,z)].ceilingTransparent;
}
bool TCOD_map3_is_floor_transparent(TCOD_map3_t map, int x, int y, int z) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return false;
	TCOD_IFNOT((unsigned)x <(unsigned)m->width && (unsigned)y < (unsigned)m->height && (unsigned)z < (unsigned)m->depth) return false;
	return m->cells[MAP_COORD3(m,x,y,z)].floorTransparent;
}

bool TCOD_map3_is_walkable(TCOD_map3_t map, int x, int y, int z) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return false;
	TCOD_IFNOT((unsigned)x <(unsigned)m->width && (unsigned)y < (unsigned)m->height && (unsigned)z < (unsigned)m->depth) return false;
	return m->cells[MAP_COORD3(m,x,y,z)].walkable;
}
bool TCOD_map3_is_down_ramp(TCOD_map3_t map, int x, int y, int z) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return false;
	TCOD_IFNOT((unsigned)x <(unsigned)m->width && (unsigned)y < (unsigned)m->height && (unsigned)z < (unsigned)m->depth) return false;
	return m->cells[MAP_COORD3(m,x,y,z)].rampDown;
}
bool TCOD_map3_is_up_ramp(TCOD_map3_t map, int x, int y, int z) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return false;
	TCOD_IFNOT((unsigned)x <(unsigned)m->width && (unsigned)y < (unsigned)m->height && (unsigned)z < (unsigned)m->depth) return false;
	return m->cells[MAP_COORD3(m,x,y,z)].rampUp;
}

int TCOD_map3_get_width(TCOD_map3_t map) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return 0;
	return m->width;
}

int TCOD_map3_get_height(TCOD_map3_t map) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return 0;
	return m->height;
}
int TCOD_map3_get_depth(TCOD_map3_t map) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return 0;
	return m->depth;
}

int TCOD_map3_get_nb_cells(TCOD_map3_t map) {
	map3_t *m = (map3_t *)map;
	TCOD_IFNOT(m != NULL) return 0;
	return m->nbcells;
}

