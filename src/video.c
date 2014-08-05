#include <video.h>
#include <constants.h>
#include <geometry.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_video.h>

#include <GL/gl.h>

SDL_Surface* video__surface = NULL;

struct video__screen_extents_t video__screen_extents = {0, 0, FALSE};

const struct video__screen_extents_t* video__get_video_extents() {
  return &video__screen_extents;
}

int video__setup( uint32_t width, uint32_t height, int fullscreen ) {

  int result = SUCCESS;

  video__screen_extents.width = width;
  video__screen_extents.height = height;
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
    glOrtho(0, video__screen_extents.width, video__screen_extents.height, 0, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0, 1.0, 1.0, 1.0);

    result = video__surface ? SUCCESS : VIDEO__COULD_NOT_CREATE_SURFACE;
  }

  return result;
}

int video__teardown() {
  SDL_Quit();

  return SUCCESS;
}

int video__clearscreen(){
  glClearColor(1.0, 0.0f, 1.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

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
			 struct video__texture_data_t* texture_data_ptr){
  uint32_t red_mask = 0;
  uint32_t green_mask = 0;
  uint32_t blue_mask = 0;
  uint32_t alpha_mask = 0;

  SDL_Surface* img_surface = NULL;
  SDL_Surface* alpha_surface = NULL;
  SDL_Surface* rgba_surface = NULL;

  texture_data_ptr->texture_id = 0;
  texture_data_ptr->width = 0;
  texture_data_ptr->height = 0;

  img_surface = IMG_Load(file);
  
  if ( !img_surface ){
    fprintf(stderr, "image not loaded\n");
  }

  alpha_surface = SDL_DisplayFormatAlpha(img_surface);

  if ( !alpha_surface ){
    fprintf(stderr, "alpha channel not established\n");
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
  }

  SDL_SetAlpha(alpha_surface, 0, 0);

  if ( SUCCESS != SDL_BlitSurface(alpha_surface, NULL, rgba_surface, NULL) ){
    fprintf(stderr, "cannot blit image to buffer: %s\n", SDL_GetError());
  }

  texture_data_ptr->width = rgba_surface->w;
  texture_data_ptr->height = rgba_surface->h;

  glGenTextures(1, &(texture_data_ptr->texture_id) );
  glBindTexture(GL_TEXTURE_2D , texture_data_ptr->texture_id);
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

int video__teardown_texture(struct video__texture_data_t* texture_data_ptr){
  glDeleteTextures(1, &(texture_data_ptr->texture_id));

  texture_data_ptr->texture_id = 0;
  texture_data_ptr->width = 0;
  texture_data_ptr->height = 0;

  return SUCCESS;
}

int video__blit(const struct video__texture_data_t* texture_data_ptr, 
		const struct geo__rect_t* src,
		const struct geo__rect_t* dest){

  float src_x = (float)src->x / (float)(texture_data_ptr->width);
  float src_y = (float)src->y / (float)(texture_data_ptr->height);

  float src_x2 = src_x + (float)src->width / (float)(texture_data_ptr->width);
  float src_y2 = src_y + (float)src->height / (float)(texture_data_ptr->height);

  glBindTexture(GL_TEXTURE_2D, texture_data_ptr->texture_id);

  glBegin(GL_QUADS);
  glTexCoord2f(src_x, src_y);
  glVertex2i(dest->x, 
	     dest->y);

  glTexCoord2f(src_x2, src_y);
  glVertex2i(dest->x + dest->width, 
	     dest->y);

  glTexCoord2f(src_x2, src_y2);  
  glVertex2i(dest->x + dest->width, 
	     dest->y + dest->height);

  glTexCoord2f(src_x, src_y2);
  glVertex2i(dest->x, 
	     dest->y + dest->height);
  
  glEnd();

  return SUCCESS;
}


