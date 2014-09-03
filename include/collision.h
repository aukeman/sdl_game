#ifndef COLLISION_H
#define COLLISION_H

#include <geometry.h>
#include <types.h>

bool_t collision__rectangles_overlap( const geo__rect_t* a,
				      const geo__rect_t* b );

bool_t collision__point_in_rectangle( const geo__point_t* point,
				      const geo__rect_t* rect );

bool_t collision__point_on_line( const geo__point_t* point,
				 const geo__line_t* line );

bool_t collision__line_intersects_line( const geo__line_t* a,
					const geo__line_t* b,
					geo__point_t* intersection );

bool_t collision__line_intersects_rectangle( const geo__line_t* line,
					     const geo__rect_t* rect,
					     geo__point_t* intersection);


bool_t collision__moving_box_intersects_box( const geo__rect_t* a,
					     const geo__vector_t* a_motion,
					     const geo__rect_t* b,
					     int* distance_until_collision );

#endif
