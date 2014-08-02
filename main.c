#include <video.h>
#include <events.h>
#include <constants.h>

#include <unistd.h>

void on_quit( events__type_e type, 
	      const events__event_parameter_t* parameter, 
	      void* context ) {

  *((int*)context) = FALSE;
}

int main( int argc, char** argv ) {

  video__setup(640, 480, FALSE);

  int keep_looping = TRUE;

  events__set_callback( EVENTS__TYPE_QUIT, on_quit, &keep_looping );

  struct video__texture_data_t texture;
  video__setup_texture("testing.png", &texture);
  
  struct geo__rect_t source = {32, 32, -32, -32};
  struct geo__rect_t dest = {64, 32, 128, 128};

  js__setup();

  while ( keep_looping ) {
    events__process_events();

    video__clearscreen();

    video__blit(&texture, &source, &dest);

    video__flip();
  }

  js__teardown();

  video__teardown();

  return 0;
}
