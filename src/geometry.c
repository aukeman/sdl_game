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


