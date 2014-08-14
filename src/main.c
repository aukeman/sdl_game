#include <video.h>
#include <events.h>
#include <constants.h>
#include <timing.h>
#include <font.h>

#include <unistd.h>
#include <stdio.h>

void on_quit( events__type_e type, 
	      const events__event_parameter_t* parameter, 
	      void* context ) {

  if ( type == EVENTS__TYPE_QUIT ||
       (type == EVENTS__TYPE_KEYUP &&
	parameter->key.value == EVENTS__KEY_ESCAPE) ) {
    *((int*)context) = FALSE;
  }
}

int main( int argc, char** argv ) {

  video__setup(800, 600, 400, 300, FALSE);
  js__setup();
  timing__setup();

  int keep_looping = TRUE;

  events__set_callback( EVENTS__TYPE_QUIT, on_quit, &keep_looping );
  events__set_callback( EVENTS__TYPE_KEYUP, on_quit, &keep_looping );

  struct video__texture_handle_t* texture;
  video__setup_texture("resources/img/testing.png", &texture);
  
  geo__rect_t source = {32, 32, -32, -32};
  geo__rect_t dest = {64, 32, 128, 128};

  struct font__handle_t* font = NULL;
  fprintf( stderr, "font rc: %d\n",
	   font__create("resources/font/test_font.dat", &font) );

  while ( keep_looping ) {

    timing__declare_top_of_frame();

    events__process_events();

    video__clearscreen();

    video__blit(texture, &source, &dest);
    
    font__draw_string(font, 0, 0, 
		      "FPS:         %5.1f\n"
		      "Frame Count: %5d\n"
		      "Fame Length: %5d",
		      timing__get_instantaneous_fps(),
		      timing__get_frame_count(),
		      timing__get_frame_length());

    video__flip();
  }

  fprintf( stdout, 
	   "%d frames; %f seconds %f fps average %f fps instantaneous\n",
	   timing__get_frame_count(),
	   timing__get_top_of_frame() / 1000.0f,
	   timing__get_average_fps(),
	   timing__get_instantaneous_fps());
	   
  font__free(font);

  
  timing__teardown();
  js__teardown();
  video__teardown();

  return 0;
}
