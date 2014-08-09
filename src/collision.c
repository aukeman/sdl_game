#include <collision.h>
#include <geometry.h>
#include <constants.h>
#include <math.h>
#include <limits.h>

bool_t collision__rectangles_overlap( const geo__rect_t* a,
				      const geo__rect_t* b ){

  geo__rect_t local_a = *a;
  geo__rect_t local_b = *b;

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

bool_t collision__point_in_rectangle( const geo__point_t* point,
				      const geo__rect_t* rect ){
  geo__rect_t point_as_rect = { point->x, point->y, 0, 0 };
  
  return collision__rectangles_overlap( rect, &point_as_rect );
}

bool_t collision__line_intersects_line( const geo__line_t* a,
					const geo__line_t* b,
					geo__point_t* intersection ){

  bool_t result = FALSE;

  if ( intersection ){
    intersection->x = 0;
    intersection->y = 0;
  }

  if ( a->x1 == a->x2 && b->x1 == b->x2 ){
    /* two parallel vertical lines */
    result = FALSE;
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
  
    if ( 0.0001f < fabs(slope_b - slope_a) ){

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

bool_t collision__line_intersects_rectangle( const geo__line_t* line,
					     const geo__rect_t* rect,
					     geo__point_t* intersection ){

  bool_t result = FALSE;

  geo__rect_t line_bbox = { line->x1, line->y1, 
			    line->x2 - line->x1, line->y2 - line->y1 };

  if (intersection){
    intersection->x = 0;
    intersection->y = 0;
  }

  if ( collision__rectangles_overlap( rect, &line_bbox ) ){

    geo__line_t side_1 = { rect->x+rect->width, rect->y,
			   rect->x, rect->y };
    
    geo__line_t side_2 = { rect->x+rect->width, rect->y, 
			   rect->x+rect->width, rect->y+rect->height };
    
    geo__line_t side_3 = { rect->x, rect->y+rect->height,
			   rect->x+rect->width, rect->y+rect->height };
    
    geo__line_t side_4 = { rect->x, rect->y+rect->height, 
			   rect->x, rect->y };

    geo__line_t* sides[] = { &side_1, &side_2, &side_3, &side_4 };

    geo__point_t current_intersection = {0, 0};

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
  }

  return result;
}

