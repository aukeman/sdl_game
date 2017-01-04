#ifndef CAMERA_H
#define CAMERA_H

#include <geometry.h>
#include <types.h>

struct geo__point_t;
struct geo__rect_t;

struct camera_t {
  struct geo__point_t position;
  struct geo__rect_t viewport;

  int32_t min_world_x;
  int32_t min_world_y;
  int32_t max_world_x;
  int32_t max_world_y;

  int32_t viewport_half_width;
  int32_t viewport_half_height;
};

struct camera__render_params_t{
  int32_t translate_x;
  int32_t translate_y;
};

int camera__setup( struct camera_t*,
		   const struct geo__point_t* position,
		   const struct geo__vector_t* viewport_dimensions,
		   const struct geo__rect_t* world_bounds );

int camera__teardown(struct camera_t*);

int camera__set_position( struct camera_t*, const struct geo__point_t* );
const struct geo__point_t* camera__get_position(const struct camera_t*);

const struct geo__rect_t* camera__get_viewport(const struct camera_t*);

int camera__move_to( struct camera_t*, int32_t x, int32_t y );

int camera__center_on( struct camera_t*, const struct geo__point_t* );

int camera__begin_render(const struct camera_t*, 
			 struct camera__render_params_t*);

int camera__begin_render_parallax(const struct camera_t*, 
				  float parallax_x,
				  float parallax_y,
				  struct camera__render_params_t*);


int camera__end_render(const struct camera_t*, struct camera__render_params_t*);

#endif
