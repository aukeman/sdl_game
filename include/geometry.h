#ifndef GEOMETRY_H
#define GEOMETRY_H

struct geo__rect_t{
  int x;
  int y;

  int width;
  int height;
};

struct geo__point_t{
  int x;
  int y;
};

struct geo__vector_t{
  int x;
  int y;
};

struct geo__line_t{
  int x1;
  int y1;

  int x2;
  int y2;
};

void geo__init_point( struct geo__point_t*, int x, int y );

void geo__init_rect( struct geo__rect_t*, int x, int y, int width, int height );

void geo__init_line( struct geo__line_t*, int x1, int y1, int x2, int y2 );

int geo__distance( const struct geo__point_t* a, const struct geo__point_t* b );
int geo__distance_squared( const struct geo__point_t* a, const struct geo__point_t* b );

int geo__length( const struct geo__line_t* l );
int geo__length_squared( const struct geo__line_t* l );

void geo__point_on_line_at_distance( const struct geo__line_t* l, int distance, struct geo__point_t* p );

#endif
