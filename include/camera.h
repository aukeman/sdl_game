#ifndef CAMERA_H
#define CAMERA_H

#include <types.h>

struct geo__point_t;
struct geo__rect_t;

int camera__setup( const struct geo__point_t* position,
		   const struct geo__rect_t* viewport,
		   const struct geo__rect_t* bounds );

int camera__teardown();

int camera__set_position( const struct geo__point_t* );
const struct geo__point_t* camera__get_position();

int camera__move( uint32_t dx, uint32_t dy );
int camera__move_to( uint32_t x, uint32_t y );

int camera__center_on( const struct geo__point_t* );

int camera__begin_render();
int camera__end_render();

#endif
