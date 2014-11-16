#include <collision.h>
#include <geometry.h>
#include <utils.h>
#include <constants.h>
#include <math.h>
#include <limits.h>

static 
bool_t __parallel_line_collision( const struct geo__line_t* a, 
				  const struct geo__line_t* b, 
				  struct geo__point_t* intersection );

static
uint32_t __distance_squared( const struct geo__point_t* a,
			     const struct geo__point_t* b );


bool_t collision__rectangles_overlap( const struct geo__rect_t* a,
				      const struct geo__rect_t* b ){

  struct geo__rect_t local_a = *a;
  struct geo__rect_t local_b = *b;

  if ( local_a.width < 0 )
  {
    local_a.x += a->width;
    local_a.width = -a->width;
  }

  if ( local_a.height < 0 )
  {
    local_a.y += a->height;
    local_a.height = -a->height;
  }

  if ( local_b.width < 0 )
  {
    local_b.x += b->width;
    local_b.width = -b->width;
  }

  if ( local_b.height < 0 )
  {
    local_b.y += b->height;
    local_b.height = -b->height;
  }

  return ( local_a.x <= (local_b.x + local_b.width)  &&
	   local_b.x <= (local_a.x + local_a.width)  &&
	   local_a.y <= (local_b.y + local_b.height) &&
	   local_b.y <= (local_a.y + local_a.height) );
}

bool_t collision__touches_top( const struct geo__rect_t* toucher, 
			       const struct geo__rect_t* touchee ){
  return ( toucher->y + toucher->height == touchee->y);
}

bool_t collision__touches_bottom( const struct geo__rect_t* toucher, 
				  const struct geo__rect_t* touchee ){
  return collision__touches_top( touchee, toucher );
}

bool_t collision__touches_left( const struct geo__rect_t* toucher, 
				const struct geo__rect_t* touchee ){
  return ( toucher->x + toucher->width == touchee->x);
}

bool_t collision__touches_right( const struct geo__rect_t* toucher, 
				 const struct geo__rect_t* touchee ){
  return collision__touches_left( touchee, toucher );
}

bool_t collision__point_in_rectangle( const struct geo__point_t* point,
				      const struct geo__rect_t* rect ){

  struct geo__rect_t point_as_rect = { point->x, point->y, 0, 0 };
  
  return collision__rectangles_overlap( rect, &point_as_rect );
}

bool_t collision__point_on_line( const struct geo__point_t* point,
				 const struct geo__line_t* line ){
  
  bool_t result = FALSE;

  if ( line->x1 <= point->x && point->x <= line->x2 ||
       line->x2 <= point->x && point->x <= line->x1 ){

    if ( line->y1 == line->y2 && point->y == line->y1 ){
      result = TRUE;
    }
    else if ( line->x1 == line->x2 && point->x == line->x1 ){
      result = (( line->y1 <= point->y && point->y <= line->y2 ) ||
		( line->y2 <= point->y && point->y <= line->y1 ));
    }
    else{
      float slope = (float)(line->y2 - line->y1) / (line->x2 - line->x1);
      float y_intercept = (line->y2 - slope*line->x2);

      float y_at_point_x = slope*point->x + y_intercept;

      result = (fabs(y_at_point_x - point->y) < 0.5f);
    }
  }

  return result;
}

bool_t collision__line_intersects_line( const struct geo__line_t* a,
					const struct geo__line_t* b,
					struct geo__point_t* intersection ){

  bool_t result = FALSE;

  if ( intersection ){
    intersection->x = 0;
    intersection->y = 0;
  }

  if ( a->x1 == a->x2 && b->x1 == b->x2 ){
    result = __parallel_line_collision( a, b, intersection );
  }
  else if ( a->x1 == a->x2 ) {
    /* a is vertical */

    result = collision__line_intersects_line( b, a, intersection );
  }
  else if ( b->x1 == b->x2 ) {
    /* b is vertical */

    float slope_a = (float)(a->y2 - a->y1) / (a->x2 - a->x1);

    float y_at_intercept = slope_a * b->x1 + ( a->y2 - slope_a*a->x2);

    bool_t x_on_line_a = 
      ((a->x1 <= b->x1 && b->x1 <= a->x2) ||
       (a->x2 <= b->x1 && b->x1 <= a->x1));
    
    bool_t y_on_line_b = 
      ((b->y1 <= y_at_intercept && y_at_intercept <= b->y2) ||
       (b->y2 <= y_at_intercept && y_at_intercept <= b->y1));

    result = (x_on_line_a && y_on_line_b);

    if ( intersection ){
      intersection->x = b->x1;
      intersection->y = (int)rintf(y_at_intercept);
    }
  }
  else {

    float slope_a = (float)(a->y2 - a->y1) / (a->x2 - a->x1);
    float slope_b = (float)(b->y2 - b->y1) / (b->x2 - b->x1);
  
    if (fabs(slope_b - slope_a) < 0.0001f ){
      result = __parallel_line_collision( a, b, intersection );
    }
    else{

      float x_at_intercept = 
	((a->y2 - slope_a*a->x2) - (b->y2 - slope_b*b->x2)) / (slope_b - slope_a);

      bool_t x_on_line_a = 
	((a->x1 <= x_at_intercept && x_at_intercept <= a->x2) ||
	 (a->x2 <= x_at_intercept && x_at_intercept <= a->x1));

      bool_t x_on_line_b = 
	((b->x1 <= x_at_intercept && x_at_intercept <= b->x2) ||
	 (b->x2 <= x_at_intercept && x_at_intercept <= b->x1));

      result = ( x_on_line_a && x_on_line_b );

      if ( intersection ){
	float y_at_intercept = 
	  slope_a * x_at_intercept + (a->y2 - slope_a*a->x2);
	
	intersection->x = (int)rintf(x_at_intercept);
	intersection->y = (int)rintf(y_at_intercept);
      }
    }
  }

  return result;
}

bool_t collision__line_intersects_rectangle( const struct geo__line_t* line,
					     const struct geo__rect_t* rect,
					     struct geo__point_t* intersection ){

  bool_t result = FALSE;

  struct geo__rect_t line_bbox = { line->x1, line->y1, 
				   line->x2 - line->x1, line->y2 - line->y1 };

  if (intersection){
    intersection->x = 0;
    intersection->y = 0;
  }

  if ( collision__rectangles_overlap( rect, &line_bbox ) ){

    struct geo__line_t side_1 = { rect->x+rect->width, rect->y,
				  rect->x, rect->y };
    
    struct geo__line_t side_2 = { rect->x+rect->width, rect->y, 
				  rect->x+rect->width, rect->y+rect->height };
    
    struct geo__line_t side_3 = { rect->x, rect->y+rect->height,
				  rect->x+rect->width, rect->y+rect->height };
    
    struct geo__line_t side_4 = { rect->x, rect->y+rect->height, 
				  rect->x, rect->y };

    struct geo__line_t* sides[] = { &side_1, &side_2, &side_3, &side_4 };

    struct geo__point_t current_intersection = {0, 0};

    int idx;
    for ( idx = 0; idx < 4 && !result; ++idx ){

      int dot_product_line_with_side_normal = 
	((line->x2-line->x1)*(sides[idx]->y2-sides[idx]->y1) + 
	 (line->y2-line->y1)*(sides[idx]->x2-sides[idx]->x1));

      if ( dot_product_line_with_side_normal < 0 &&
	   collision__line_intersects_line( line, 
					    sides[idx], 
					    &current_intersection ) ){

	if ( intersection ){
	  *intersection = current_intersection;
	}

	result = TRUE;
      }
    }

    if ( !result )
    {
      struct geo__point_t origin = { line->x1, line->y1 };

      if ( collision__point_in_rectangle( &origin, rect ) ) {
	result = TRUE;
	
	if ( intersection ){
	  *intersection = origin;
	}
      }
    }
  }

  return result;
}

bool_t collision__moving_rectangle_intersects_rectangle( 
					       const struct geo__rect_t* a,
					       const struct geo__vector_t* a_motion,
					       const struct geo__rect_t* b,
					       int* distance_until_collision ){

  bool_t result = FALSE;

  if ( distance_until_collision ){
    *distance_until_collision = INT_MAX;
  }

  const int amx = a_motion->x;
  const int amy = a_motion->y;

  const int ax = a->x;
  const int ay = a->y;
  const int w = a->width;
  const int h = a->height;

  struct geo__line_t motion_lines[] = 
    { { ax,     ay,        ax + amx,     ay + amy },
      { ax + w, ay,        ax + w + amx, ay + amy },
      { ax,     a->y + h,  ax + amx,     a->y + h + amy },
      { ax + w, a->y + h,  ax + w + amx, a->y + h + amy } };

  int idx;
  for ( idx = 0; idx < 4; ++idx ){

    struct geo__point_t cp;

    if ( collision__line_intersects_rectangle( &motion_lines[idx],
					       b,
					       &cp ) ){
      result = TRUE;

      if ( distance_until_collision ){

	struct geo__point_t start = { motion_lines[idx].x1, motion_lines[idx].y1 };

	int d_sqrd = geo__distance_squared( &start, &cp );

	if ( d_sqrd < *distance_until_collision ){
	  *distance_until_collision = d_sqrd;
	}
      }
      else{
	return TRUE;
      }
    }
  }

  if ( result ){
    *distance_until_collision = utils__sqrt(*distance_until_collision);
  }
  
  return result;
}


bool_t __parallel_line_collision( const struct geo__line_t* a, 
				  const struct geo__line_t* b, 
				  struct geo__point_t* intersection ){


  struct geo__point_t a1 = { a->x1, a->y1 };
  
  bool_t result = FALSE;

  if ( collision__point_on_line( &a1, b ) ){
    if ( intersection ){
      *intersection = a1;
    }

    result = TRUE;
  }
  else {

    struct geo__point_t b1 = { b->x1, b->y1 };
    struct geo__point_t b2 = { b->x2, b->y2 };

    bool_t b1_on_a = collision__point_on_line(&b1, a);
    bool_t b2_on_a = collision__point_on_line(&b2, a);

    if ( b1_on_a && b2_on_a ){

      if ( intersection ){

	uint32_t distance_to_b1 = geo__distance_squared( &a1, &b1 );
	uint32_t distance_to_b2 = geo__distance_squared( &a1, &b2 );

	*intersection = distance_to_b1 < distance_to_b2 ? b1 : b2;
      }
      
      result = TRUE;

      /* shortest distance to colliding point */
    }
    else if ( b1_on_a ){
      if ( intersection ){
	*intersection = b1;
      }

      result = TRUE;
    }
    else if ( b2_on_a ){
      if ( intersection ){
	*intersection = b2;
      }

      result = TRUE;
    }
  }

  return result;
}

