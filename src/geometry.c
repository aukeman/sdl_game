#include <geometry.h>
#include <utils.h>


#include <math.h>

void geo__init_rect( struct geo__rect_t* dest, int x, int y, int width, int height ){
  dest->x = x;
  dest->y = y;
  dest->width = width;
  dest->height = height;
}

void geo__copy_rect( struct geo__rect_t* dest, const struct geo__rect_t* src ){
  geo__init_rect( dest, src->x, src->y, src->width, src->height );
}

int geo__distance( const struct geo__point_t* a, const struct geo__point_t* b ){
  int d_sqrd = geo__distance_squared(a, b);

  return utils__sqrt(d_sqrd);
}

int geo__distance_squared( const struct geo__point_t* a, const struct geo__point_t* b ){
  
  int x_len = ((a->x)-(b->x));
  int y_len = ((a->y)-(b->y));

  return (x_len*x_len + y_len*y_len);
}

int geo__length( const struct geo__line_t* l ){
  int d_sqrd = geo__length_squared(l);

  return utils__sqrt(d_sqrd);
}

int geo__length_squared( const struct geo__line_t* l ){
  struct geo__point_t a = { l->x1, l->y1 };
  struct geo__point_t b = { l->x2, l->y2 };

  return geo__distance_squared( &a, &b );
}

void geo__point_on_line_at_distance( const struct geo__line_t* l, 
				     int distance, 
				     struct geo__point_t* p ){

  int line_length = geo__length( l );

  if ( 0 < line_length ){
    p->x = (l->x2 - l->x1)*distance/line_length + l->x1;
    p->y = (l->y2 - l->y1)*distance/line_length + l->y1;
  }
  else{
    p->x = l->x1;
    p->y = l->y1;
  }
}
