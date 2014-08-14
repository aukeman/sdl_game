#ifndef VIDEO_H
#define VIDEO_H

#include <geometry.h>

#include <stdint.h>

enum {
  VIDEO__ERROR_CODE_BASE = 0x0,
  VIDEO__COULD_NOT_CREATE_SURFACE,
  VIDEO__COULD_NOT_INITIALIZE_SDL,

  VIDEO__COULD_NOT_LOAD_IMAGE,

  VIDEO__ERROR_CODE_LAST
};

typedef struct {
  uint32_t width;
  uint32_t height;
  
  int fullscreen;
} video__screen_extents_t;

struct video__texture_handle_t;

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

int video__blit(const struct video__texture_handle_t* texture_handle, 
		const geo__rect_t* src,
		const geo__rect_t* dest);

int video__rect(const geo__rect_t* rect,
		uint8_t red,
		uint8_t green,
		uint8_t blue,
		uint8_t alpha);

int video__line(const geo__line_t* line,
		uint8_t red,
		uint8_t green,
		uint8_t blue,
		uint8_t alpha);
		

#endif
