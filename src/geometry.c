#include <geometry.h>
#include <utils.h>


#include <math.h>


int geo__distance( const geo__point_t* a, const geo__point_t* b ){
  int d_sqrd = geo__distance_squared(a, b);

  return utils__sqrt(d_sqrd);
}

int geo__distance_squared( const geo__point_t* a, const geo__point_t* b ){
  
  int x_len = ((a->x)-(b->x));
  int y_len = ((a->y)-(b->y));

  return (x_len*x_len + y_len*y_len);
}

int geo__length( const geo__line_t* l ){
  int d_sqrd = geo__length_squared(l);

  return utils__sqrt(d_sqrd);
}

int geo__length_squared( const geo__line_t* l ){
  geo__point_t a = { l->x1, l->y1 };
  geo__point_t b = { l->x2, l->y2 };

  return geo__distance_squared( &a, &b );
}

void geo__point_on_line_at_distance( const geo__line_t* l, 
				     int distance, 
				     geo__point_t* p ){

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
