#include <collision.h>
#include <geometry.h>
#include <constants.h>
#include <math.h>

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

int collision__line_intersects_line( const geo__line_t* a,
				     const geo__line_t* b ){

  /* if a vertical line, transpose x and y coordinates to avoid singularities */
  if ( a->x1 == a->x2 || b->x1 == b->x2 ){
    geo__line_t xpose_a = {a->y1, a->x1, a->y2, a->x2};
    geo__line_t xpose_b = {b->y1, b->x1, b->y2, b->x2};
    
    return collision__line_intersects_line( &xpose_a, &xpose_b );
  }

  float slope_a = (float)(a->y2 - a->y1) / (a->x2 - a->x1);
  float slope_b = (float)(b->y2 - b->y1) / (b->x2 - b->x1);
  
  /* parallel or nearly so, no intersection */
  if ( fabs(slope_b - slope_a) < 0.001f ){
    return FALSE;
  }

  float x_at_intercept = 
    (a->y2 - slope_a*a->x2 - b->y2 - slope_b*b->x2) / (slope_b - slope_a);

  int x_on_line_a = ((a->x1 < x_at_intercept && x_at_intercept < a->x2) ||
		     (a->x2 < x_at_intercept && x_at_intercept < a->x1));

  int x_on_line_b = ((b->x1 < x_at_intercept && x_at_intercept < b->x2) ||
		     (b->x2 < x_at_intercept && x_at_intercept < b->x1));


  return ( x_on_line_a && x_on_line_b );
}

int collision__line_intersects_rectangle( const geo__line_t* line,
					  const geo__rect_t* rect ){

  geo__rect_t line_bbox = { line->x1, line->y1, 
			    line->x2 - line->x1, line->y2 - line->y1 };

  geo__line_t side_1 = { rect->x, rect->y, 
			 rect->x+rect->width, rect->y };

  geo__line_t side_2 = { rect->x+rect->width, rect->y, 
			 rect->x+rect->width, rect->y+rect->height };

  geo__line_t side_3 = { rect->x+rect->width, rect->y+rect->height, 
			 rect->x, rect->y+rect->height };

  geo__line_t side_4 = { rect->x, rect->y+rect->height, 
			 rect->x, rect->y };

  return (collision__rectangles_overlap( rect, &line_bbox ) &&
	  collision__line_intersects_line( line, &side_1 ) ||
	  collision__line_intersects_line( line, &side_2 ) ||
	  collision__line_intersects_line( line, &side_3 ) ||
	  collision__line_intersects_line( line, &side_4 ));
}

