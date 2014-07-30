#include <video.h>
#include <constants.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <GL/gl.h>

SDL_Surface* video__surface = NULL;

struct video__screen_extents_t video__screen_extents = {640, 480};

const struct video__screen_extents_t* video__get_video_extents() {
  return &video__screen_extents;
}

uint32_t video__last_texture_id = 0xFFFFFFFF;

int video__setup() {

  int result = SUCCESS;

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
      SDL_SetVideoMode(video__screen_extents.w, 
		       video__screen_extents.h, 
		       0, 
		       SDL_OPENGL);

    /*
     * Set up OpenGL for 2D rendering.
     */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, video__screen_extents.w, video__screen_extents.h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, video__screen_extents.w, video__screen_extents.w, 0, -1.0, 1.0);
    
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
  glClearColor(0, 0, 0, 1.0);
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
  SDL_Surface* rgb_surface = NULL;

  texture_data_ptr->texture_id = 0;
  texture_data_ptr->width = 0;
  texture_data_ptr->height = 0;

  img_surface = IMG_Load(file);
  rgb_surface = SDL_CreateRGBSurface( SDL_SWSURFACE, 
				      img_surface->w, 
				      img_surface->h, 
				      img_surface->format->BitsPerPixel,
				      RED_MASK,
				      GREEN_MASK,
				      BLUE_MASK,
				      ALPHA_MASK );

  SDL_BlitSurface(img_surface, 0, rgb_surface, 0);

  glGenTextures(1, &(texture_data_ptr->texture_id) );
  glBindTexture(GL_TEXTURE_2D , texture_data_ptr->texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, rgb_surface->pitch / rgb_surface->format->BytesPerPixel);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2, rgb_surface->w, rgb_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb_surface->pixels);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  SDL_FreeSurface(rgb_surface);
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
		uint32_t x, 
		uint32_t y){

  glBindTexture(GL_TEXTURE_2D, texture_data_ptr->texture_id);

  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex2f(x, y);

  glTexCoord2f(1.0f, 0);
  glVertex2f((float)(texture_data_ptr->width)+x, y);

  glTexCoord2f(1.0f, 1.0f);  
  glVertex2f((float)(texture_data_ptr->width)+x, 
	     (float)(texture_data_ptr->height)+y);

  glTexCoord2f(0, 1.0f);
  glVertex2f(x, (float)(texture_data_ptr->height)+y);
  glEnd();
  
  return SUCCESS;
}


