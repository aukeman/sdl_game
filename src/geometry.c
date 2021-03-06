#include <geometry.h>
#include <utils.h>


#include <math.h>

void geo__init_point( struct geo__point_t* dest, int x, int y ){
  dest->x = x;
  dest->y = y;
}

void geo__init_vector( struct geo__vector_t* dest, int x, int y ){
  dest->x = x;
  dest->y = y;
}

void geo__init_rect( struct geo__rect_t* dest, int x, int y, int width, int height ){
  dest->x = x;
  dest->y = y;
  dest->width = width;
  dest->height = height;
}

void geo__init_line( struct geo__line_t* dest, int x1, int y1, int x2, int y2 ){
  dest->x1 = x1;
  dest->y1 = y1;
  dest->x2 = x2;
  dest->y2 = y2;
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
  struct geo__point_t a;
  struct geo__point_t b;

  a.x = l->x1;
  a.y = l->y1;

  b.x = l->x2;
  b.y = l->y2;

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
