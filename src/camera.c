#include <camera.h>
#include <geometry.h>
#include <video.h>
#include <utils.h>
#include <constants.h>

#include <stdio.h>

int camera__setup( struct camera_t* camera,
		   const struct geo__point_t* position,
		   const struct geo__vector_t* viewport_dimensions,
		   const struct geo__rect_t* world_bounds ){
  camera->position = *position;

  camera->viewport_half_width = utils__screen2pos(viewport_dimensions->x/2);
  camera->viewport_half_height = utils__screen2pos(viewport_dimensions->y/2);

  geo__init_rect( &camera->viewport, 
		  position->x, 
		  position->y,
		  2*camera->viewport_half_width,
		  2*camera->viewport_half_height );
  

  camera->min_world_x = world_bounds->x;
  camera->min_world_y = world_bounds->y;
  camera->max_world_x = world_bounds->x + world_bounds->width - camera->viewport.width;
  camera->max_world_y = world_bounds->y + world_bounds->height - camera->viewport.height;

  return SUCCESS;
}

int camera__teardown(struct camera_t* camera){
  camera->position.x = 0;
  camera->position.y = 0;

  geo__init_rect(&camera->viewport, 0, 0, 0, 0);

  camera->viewport_half_width = 0;
  camera->viewport_half_height = 0;
  
  camera->min_world_x = 0;
  camera->min_world_y = 0;
  camera->max_world_x = 0;
  camera->max_world_y = 0;
    

  return SUCCESS;
}

int camera__set_position( struct camera_t* camera, 
			  const struct geo__point_t* position){
  camera__move_to( camera, position->x, position->y );
  return SUCCESS;
}

const struct geo__point_t* camera__get_position(const struct camera_t* camera){
  return &camera->position;
}

int camera__move_to( struct camera_t* camera, int32_t x, int32_t y ){

  camera->position.x = 
    camera->viewport.x = utils__clamp( camera->min_world_x, 
				       camera->max_world_x,
				       x );

  camera->position.y = 
    camera->viewport.y = utils__clamp( camera->min_world_y,
				       camera->max_world_y,
				       y );

  return SUCCESS;
}

int camera__center_on( struct camera_t* camera, 
		       const struct geo__point_t* center ){
  
  return camera__move_to( camera,
			  center->x - camera->viewport_half_width,
			  center->y - camera->viewport_half_height );
}

int camera__begin_render( const struct camera_t* camera, struct camera__render_params_t* params ){
  params->translate_x = -utils__pos2screen( camera->position.x );
  params->translate_y = -utils__pos2screen( camera->position.y );

  return video__translate( params->translate_x, 
			   params->translate_y );
}

int camera__begin_render_parallax( const struct camera_t* camera,
				   float parallax_x,
				   float parallax_y,
				   struct camera__render_params_t* params){

  params->translate_x = -utils__pos2screen( utils__round(camera->position.x*parallax_x) );
  params->translate_y = -utils__pos2screen( utils__round(camera->position.y*parallax_y) );

  return video__translate( params->translate_x, 
			   params->translate_y );
}

int camera__end_render( const struct camera_t* camera, struct camera__render_params_t* params ){

  int result = video__translate( -params->translate_x, 
				 -params->translate_y );

  params->translate_x = 0;
  params->translate_y = 0;

  return result;
}





