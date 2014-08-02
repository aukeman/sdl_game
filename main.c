#include <video.h>
#include <events.h>
#include <constants.h>
#include <timing.h>

#include <unistd.h>
#include <stdio.h>

void on_quit( events__type_e type, 
	      const events__event_parameter_t* parameter, 
	      void* context ) {

  *((int*)context) = FALSE;
}

int main( int argc, char** argv ) {

  video__setup(640, 480, FALSE);
  js__setup();
  timing__setup();

  int keep_looping = TRUE;

  events__set_callback( EVENTS__TYPE_QUIT, on_quit, &keep_looping );

  struct video__texture_data_t texture;
  video__setup_texture("testing.png", &texture);
  
  struct geo__rect_t source = {32, 32, -32, -32};
  struct geo__rect_t dest = {64, 32, 128, 128};

  while ( keep_looping ) {

    timing__declare_top_of_frame();

    events__process_events();

    video__clearscreen();

    video__blit(&texture, &source, &dest);

    video__flip();
  }

  fprintf( stdout, 
	   "%d frames; %f seconds %f fps average %f fps instantaneous\n",
	   timing__get_frame_count(),
	   timing__get_top_of_frame() / 1000.0f,
	   timing__get_average_fps(),
	   timing__get_instantaneous_fps());
	   
  
  timing__teardown();
  js__teardown();
  video__teardown();

  return 0;
}
