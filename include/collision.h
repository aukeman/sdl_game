#ifndef COLLISION_H
#define COLLISION_H

#include <geometry.h>
#include <types.h>

bool_t collision__rectangles_overlap( const struct geo__rect_t* a,
				      const struct geo__rect_t* b );

bool_t collision__touches_top( const struct geo__rect_t* toucher, 
			       const struct geo__rect_t* touchee );

bool_t collision__touches_bottom( const struct geo__rect_t* toucher, 
				  const struct geo__rect_t* touchee );

bool_t collision__touches_left( const struct geo__rect_t* toucher, 
				const struct geo__rect_t* touchee );

bool_t collision__touches_right( const struct geo__rect_t* toucher, 
				 const struct geo__rect_t* touchee );

bool_t collision__point_in_rectangle( const struct geo__point_t* point,
				      const struct geo__rect_t* rect );

bool_t collision__point_on_line( const struct geo__point_t* point,
				 const struct geo__line_t* line );

bool_t collision__line_intersects_line( const struct geo__line_t* a,
					const struct geo__line_t* b,
					struct geo__point_t* intersection );

bool_t collision__line_intersects_rectangle( const struct geo__line_t* line,
					     const struct geo__rect_t* rect,
					     struct geo__point_t* intersection);


bool_t collision__moving_rectangle_intersects_rectangle(const struct geo__rect_t* a,
							const struct geo__vector_t* a_motion,
							const struct geo__rect_t* b,
							int* distance_until_collision );

#endif
