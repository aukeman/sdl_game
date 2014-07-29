#include <video.h>
#include <constants.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

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
