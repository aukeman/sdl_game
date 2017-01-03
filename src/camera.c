#include <camera.h>
#include <geometry.h>
#include <video.h>
#include <utils.h>
#include <constants.h>

#include <stdio.h>

static struct geo__point_t camera_position = {0, 0};
static struct geo__rect_t camera_viewport = {0, 0, 0, 0};
static struct geo__rect_t world_bounds = {0, 0, 0, 0};


int camera__setup( const struct geo__point_t* position,
		   const struct geo__rect_t* viewport,
		   const struct geo__rect_t* bounds ){
  camera_position = *position;
  camera_viewport = *viewport;
  world_bounds = *bounds;

  return SUCCESS;
}

int camera__teardown(){
  camera_position.x = 0;
  camera_position.y = 0;

  camera_viewport.x = 0;
  camera_viewport.y = 0;
  camera_viewport.width = 0;
  camera_viewport.height = 0;

  world_bounds.x = 0;
  world_bounds.y = 0;
  world_bounds.width = 0;
  world_bounds.height = 0;
  
  return SUCCESS;
}

int camera__set_position( const struct geo__point_t* position){
  camera_position = *position;
  return SUCCESS;
}

const struct geo__point_t* camera__get_position(){
  return &camera_position;
}

int camera__move( uint32_t dx, uint32_t dy ){
  return camera__move_to( camera_position.x + dx, 
			  camera_position.y + dy );
}

int camera__move_to( uint32_t x, uint32_t y ){

  camera_position.x = utils__clamp(world_bounds.x, 
				   world_bounds.x + world_bounds.width - utils__screen2pos(camera_viewport.width), 
				   x);
  camera_position.y = utils__clamp(world_bounds.y, 
				   world_bounds.y + world_bounds.height - utils__screen2pos(camera_viewport.height), 
				   y);

  return SUCCESS;
}

int camera__center_on( const struct geo__point_t* center ){
  
  return camera__move_to( center->x - utils__screen2pos(camera_viewport.width)/2,
			  center->y - utils__screen2pos(camera_viewport.height)/2 );
}

int camera__begin_render(){

  return video__translate( -utils__pos2screen(camera_position.x), 
			   -utils__pos2screen(camera_position.y) );
}

int camera__end_render(){
  return video__translate( utils__pos2screen(camera_position.x), 
			   utils__pos2screen(camera_position.y) );
}





