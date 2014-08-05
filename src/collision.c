#include <collision.h>
#include <geometry.h>

int collision__rectangles_overlap( const geo__rect_t* a,
				   const geo__rect_t* b ){

  return ( a->x < (b->x + b->width)  &&
	   b->x < (a->x + a->width)  &&
	   a->y < (b->y + a->height) &&
	   b->y < (a->y + a->height) );

}

int collision__point_in_rectangle( const geo__point_t* point,
				   const geo__rect_t* rect ){
  geo__rect_t point_as_rect = { point->x, point->y, 0, 0 };
  
  return collision__rectangles_overlap( rect, &point_as_rect );
}

int collision__line_intersects_rectangle( const geo__line_t* line,
					  const geo__rect_t* rect ){

  geo__rect_t line_bbox = { line->x1, line->y1, 
			    line->x2 - line->x1, line->y2 - line->y1 };

  // TODO: real line/box collision
  return collision__rectangles_overlap( rect, &line_bbox );
}

