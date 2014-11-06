#include <video.h>
#include <constants.h>
#include <geometry.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_video.h>

#include <GL/gl.h>

SDL_Surface* video__surface = NULL;

uint32_t last_rendered_texture_id = 0;

struct {
  const struct video__blit_params_t* params;
  bool_t in_progress;

} blit_operation;

video__screen_extents_t video__screen_extents = {0, 0, FALSE};

struct video__texture_handle_t {
  uint32_t texture_id;
  uint32_t width;
  uint32_t height;
};

const video__screen_extents_t* video__get_screen_extents() {
  return &video__screen_extents;
}



int video__setup( uint32_t screen_width, 
		  uint32_t screen_height, 
		  uint32_t viewport_width,
		  uint32_t viewport_height,
		  int fullscreen ) {

  int result = SUCCESS;

  video__screen_extents.width = screen_width;
  video__screen_extents.height = screen_height;
  video__screen_extents.viewport_width = viewport_width;
  video__screen_extents.viewport_height = viewport_height;
  video__screen_extents.fullscreen = fullscreen;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      fprintf(stderr,
              "\nError: I could not initialize video!\n"
              "The Simple DirectMedia error that occured was:\n"
              "%s\n\n", SDL_GetError());

      result = VIDEO__COULD_NOT_INITIALIZE_SDL;
  }
  else {

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    video__surface = 
      SDL_SetVideoMode(video__screen_extents.width, 
		       video__screen_extents.height, 
		       0, 
		       SDL_HWSURFACE | 
		       SDL_OPENGL | 
		       SDL_DOUBLEBUF |
		       (fullscreen ? SDL_FULLSCREEN : 0));

    /*
     * Set up OpenGL for 2D rendering.
     */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, video__screen_extents.width, video__screen_extents.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, viewport_width, viewport_height, 0, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0, 1.0, 1.0, 1.0);

    glClearColor(1.0, 0.0f, 1.0f, 0.0f);

    result = video__surface ? SUCCESS : VIDEO__COULD_NOT_CREATE_SURFACE;
  }

  return result;
}

int video__teardown() {
  SDL_Quit();

  return SUCCESS;
}

int video__clearscreen(){
  glClear(GL_COLOR_BUFFER_BIT);

  /* static geo__rect_t screen = { 0, 0, 0, 0 }; */

  /* screen.width = video__screen_extents.width; */
  /* screen.height = video__screen_extents.height; */

  /* video__rect( &screen, 255, 0, 255, 0 ); */

  return SUCCESS;
}

int video__flip() {
  SDL_GL_SwapBuffers();

  return SUCCESS;
}

#if SDL_BYTEORDER == SDL_BIG_ENDIAN

#define RED_MASK   0xFF000000
#define GREEN_MASK 0x00FF0000
#define BLUE_MASK  0x0000FF00
#define ALPHA_MASK 0x000000FF

#else

#define RED_MASK   0x000000FF
#define GREEN_MASK 0x0000FF00
#define BLUE_MASK  0x00FF0000
#define ALPHA_MASK 0xFF000000

#endif


int video__setup_texture(const char* file, 
			 struct video__texture_handle_t** texture_handle_ptr){
  uint32_t red_mask = 0;
  uint32_t green_mask = 0;
  uint32_t blue_mask = 0;
  uint32_t alpha_mask = 0;

  SDL_Surface* img_surface = NULL;
  SDL_Surface* alpha_surface = NULL;
  SDL_Surface* rgba_surface = NULL;

  img_surface = IMG_Load(file);
  
  if ( !img_surface ){
    fprintf(stderr, "image not loaded\n");
    *texture_handle_ptr = NULL;
    return VIDEO__COULD_NOT_LOAD_IMAGE;
  }

  alpha_surface = SDL_DisplayFormatAlpha(img_surface);

  if ( !alpha_surface ){
    fprintf(stderr, "alpha channel not established\n");
    *texture_handle_ptr = NULL;
    return VIDEO__COULD_NOT_LOAD_IMAGE;
  }

  rgba_surface = SDL_CreateRGBSurface( SDL_SWSURFACE, 
				       alpha_surface->w, 
				       alpha_surface->h, 
				       alpha_surface->format->BitsPerPixel,
				       RED_MASK,
				       GREEN_MASK,
				       BLUE_MASK,
				       ALPHA_MASK );

  if ( !rgba_surface ){
    fprintf(stderr, "cannot create RGB buffer\n");
    *texture_handle_ptr;
    return VIDEO__COULD_NOT_LOAD_IMAGE;
  }

  SDL_SetAlpha(alpha_surface, 0, 0);

  if ( SUCCESS != SDL_BlitSurface(alpha_surface, NULL, rgba_surface, NULL) ){
    fprintf(stderr, "cannot blit image to buffer: %s\n", SDL_GetError());
    *texture_handle_ptr;
    return VIDEO__COULD_NOT_LOAD_IMAGE;
  }

  (*texture_handle_ptr) = 
    (struct video__texture_handle_t*)malloc(sizeof(**texture_handle_ptr));

  (*texture_handle_ptr)->width = rgba_surface->w;
  (*texture_handle_ptr)->height = rgba_surface->h;

  glGenTextures(1, &((*texture_handle_ptr)->texture_id) );
  glBindTexture(GL_TEXTURE_2D , (*texture_handle_ptr)->texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, rgba_surface->pitch / rgba_surface->format->BytesPerPixel);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2, rgba_surface->w, rgba_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba_surface->pixels);


  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  SDL_FreeSurface(rgba_surface);
  SDL_FreeSurface(alpha_surface);
  SDL_FreeSurface(img_surface);

  return SUCCESS;
}

int video__teardown_texture(struct video__texture_handle_t* texture_handle){
  glDeleteTextures(1, &(texture_handle->texture_id));

  free(texture_handle);

  return SUCCESS;
}

int video__translate( int32_t x, int32_t y ){
  glTranslatef(x, y, 0);
}

int video__begin_blits( const struct video__blit_params_t* params ){
  
  glBindTexture(GL_TEXTURE_2D, params->texture_handle->texture_id);
  last_rendered_texture_id = params->texture_handle->texture_id;

  if ( params->set_color ){
    glColor4f(params->color.red, 
	      params->color.green, 
	      params->color.blue, 
	      params->color.alpha);
  }

  if ( params->suppress_transparency ){
      glDisable(GL_BLEND);
  }

  blit_operation.params = params;
  blit_operation.in_progress = TRUE;

  glBegin(GL_QUADS);

}

int video__end_blits(){

  glEnd();
  blit_operation.in_progress = FALSE;

  if ( blit_operation.params->suppress_transparency ){
    glEnable(GL_BLEND);
  }

  if ( blit_operation.params->set_color ){
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  }

  blit_operation.params = NULL;
}

int video__blit( const geo__rect_t* src,
		 const geo__rect_t* dest ){

  if ( !blit_operation.in_progress ) {
    return UNKNOWN_FAILURE;
  }

  return video__blit_verts( 
	      (float)src->x / (float)(blit_operation.params->texture_handle->width),
	      (float)src->y / (float)(blit_operation.params->texture_handle->height),
	      (float)(src->x + src->width) / (float)(blit_operation.params->texture_handle->width),
	      (float)(src->y + src->height) / (float)(blit_operation.params->texture_handle->height),
	      dest->x, 
	      dest->y,
	      dest->x + dest->width,
	      dest->y + dest->height );	       
}

int video__blit_verts( float src_x1, float src_y1,
		       float src_x2, float src_y2,
		       int dst_x1,   int dst_y1, 
		       int dst_x2,   int dst_y2 ){
		       
  if ( !blit_operation.in_progress ) {
    return UNKNOWN_FAILURE;
  }

  glTexCoord2f(src_x1, src_y1);
  glVertex2i(dst_x1, dst_y1);

  glTexCoord2f(src_x2, src_y1);
  glVertex2i(dst_x2, dst_y1);

  glTexCoord2f(src_x2, src_y2);  
  glVertex2i(dst_x2, dst_y2);

  glTexCoord2f(src_x1, src_y2);
  glVertex2i(dst_x1, dst_y2);

  return SUCCESS;
}

int video__blit_single(const struct video__texture_handle_t* texture_handle, 
		       const geo__rect_t* src,
		       const geo__rect_t* dest){

  if ( blit_operation.in_progress ) {
    return UNKNOWN_FAILURE;
  }

  struct video__blit_params_t params = 
    { texture_handle, FALSE, { 0, 0, 0, 0 }, FALSE };

  video__begin_blits( &params );

  video__blit( src, dest );

  video__end_blits();

  return SUCCESS;
}

uint32_t video__get_texture_width( const struct video__texture_handle_t* texture_handle ){
  return texture_handle->width;
}

uint32_t video__get_texture_height( const struct video__texture_handle_t* texture_handle ){
  return texture_handle->height;
}

int video__rect(const geo__rect_t* rect,
		uint8_t red,
		uint8_t green,
		uint8_t blue,
		uint8_t alpha){

  glDisable(GL_TEXTURE_2D);
  last_rendered_texture_id = 0;

  glColor4f(red/255.0f, green/255.0f, blue/255.0f, alpha/255.0f);

  glBegin(GL_QUADS);
  glVertex2i(rect->x, 
	     rect->y);

  glVertex2i(rect->x + rect->width, 
	     rect->y);

  glVertex2i(rect->x + rect->width, 
	     rect->y + rect->height);

  glVertex2i(rect->x, 
	     rect->y + rect->height);
  
  glEnd();

  glEnable(GL_TEXTURE_2D);

  return SUCCESS;
}

int video__line(const geo__line_t* line,
		uint8_t red,
		uint8_t green,
		uint8_t blue,
		uint8_t alpha){

  glDisable(GL_TEXTURE_2D);
  last_rendered_texture_id = 0;

  glColor4f(red/255.0f, green/255.0f, blue/255.0f, alpha/255.0f);

  glBegin(GL_LINES);
  glVertex2i(line->x1, 
	     line->y1);

  glVertex2i(line->x2, 
	     line->y2);
  glEnd();

  glEnable(GL_TEXTURE_2D);

  return SUCCESS;
}

