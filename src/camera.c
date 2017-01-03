#include <camera.h>
#include <geometry.h>
#include <video.h>
#include <utils.h>
#include <constants.h>

#include <stdio.h>

int camera__setup( struct camera_t* camera,
		   const struct geo__point_t* position,
		   const struct geo__rect_t* viewport,
		   const struct geo__rect_t* bounds ){
  camera->position = *position;

  camera->viewport_half_width = utils__screen2pos(viewport->width/2);
  camera->viewport_half_height = utils__screen2pos(viewport->height/2);

  camera->min_world_x = bounds->x;
  camera->min_world_y = bounds->y;
  camera->max_world_x = bounds->x + bounds->width - 2*camera->viewport_half_width;
  camera->max_world_y = bounds->y + bounds->height - 2*camera->viewport_half_height;

  return SUCCESS;
}

int camera__teardown(struct camera_t* camera){
  camera->position.x = 0;
  camera->position.y = 0;

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

  camera->position.x = utils__clamp( camera->min_world_x, 
				     camera->max_world_x,
				     x );

  camera->position.y = utils__clamp( camera->min_world_y,
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

int camera__begin_render( const struct camera_t* camera ){

  return video__translate( -utils__pos2screen(camera->position.x), 
			   -utils__pos2screen(camera->position.y) );
}

int camera__end_render( const struct camera_t* camera ){
  return video__translate( utils__pos2screen(camera->position.x), 
			   utils__pos2screen(camera->position.y) );
}





