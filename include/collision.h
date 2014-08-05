#ifndef COLLISION_H
#define COLLISION_H

#include <geometry.h>


int collision__rectangles_overlap( const geo__rect_t* a,
				   const geo__rect_t* b );

int collision__point_in_rectangle( const geo__point_t* point,
				   const geo__rect_t* rect );

int collision__line_intersects_line( const geo__line_t* a,
				     const geo__line_t* b );

int collision__line_intersects_rectangle( const geo__line_t* point,
					  const geo__rect_t* rect );


#endif
