#ifndef VIDEO_H
#define VIDEO_H

#include <geometry.h>
#include <types.h>
#include <stdint.h>

enum {
  VIDEO__ERROR_CODE_BASE = 0x0,
  VIDEO__COULD_NOT_CREATE_SURFACE,
  VIDEO__COULD_NOT_INITIALIZE_SDL,

  VIDEO__COULD_NOT_LOAD_IMAGE,

  VIDEO__ERROR_CODE_LAST
};

typedef struct {
  uint32_t pixels_width;
  uint32_t pixels_height;
  
  uint32_t viewport_screen_width;
  uint32_t viewport_screen_height;

  uint32_t viewport_position_width;
  uint32_t viewport_position_height;

  int fullscreen;
} video__screen_extents_t;

struct video__texture_handle_t;

struct video__blit_params_t{

  const struct video__texture_handle_t* texture_handle;

  bool_t set_color;

  struct {
    float red;
    float green;
    float blue;
    float alpha;
  } color;

  bool_t suppress_transparency;

};

const video__screen_extents_t* video__get_screen_extents();

int video__setup(uint32_t screen_width, 
		 uint32_t screen_height, 
		 uint32_t viewport_width, 
		 uint32_t viewport_height, 
		 int fullscreen);
int video__teardown();

int video__clearscreen();

int video__flip();

int video__setup_texture(const char* file_directory, 
			 struct video__texture_handle_t** texture_handle_ptr );

int video__teardown_texture(struct video__texture_handle_t* texture_handle);

int video__translate( int32_t x, int32_t y );

int video__begin_blits( const struct video__blit_params_t* params );

int video__blit( const struct geo__rect_t* src,
		 const struct geo__rect_t* dest);

int video__blit_verts( float src_x1, float src_y1,
		       float src_x2, float src_y2,
		       int dst_x1,   int dst_y1, 
		       int dst_x2,   int dst_y2 );

int video__end_blits();

int video__blit_single(const struct video__texture_handle_t* texture_handle, 
		       const struct geo__rect_t* src,
		       const struct geo__rect_t* dest);

uint32_t video__get_texture_width( const struct video__texture_handle_t* texture_handle );

uint32_t video__get_texture_height( const struct video__texture_handle_t* texture_handle );

int video__rect(const struct geo__rect_t* rect,
		uint8_t red,
		uint8_t green,
		uint8_t blue,
		uint8_t alpha);

int video__line(const struct geo__line_t* line,
		uint8_t red,
		uint8_t green,
		uint8_t blue,
		uint8_t alpha);
		

#endif
