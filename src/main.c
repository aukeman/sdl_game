#include <video.h>
#include <events.h>
#include <constants.h>
#include <timing.h>
#include <font.h>
#include <control.h>

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
  control__setup("data/controls.dat");

  int keep_looping = TRUE;

  events__add_callback( EVENTS__TYPE_QUIT, on_quit, &keep_looping );
  events__add_callback( EVENTS__TYPE_KEYUP, on_quit, &keep_looping );

  struct video__texture_handle_t* texture;
  video__setup_texture("resources/img/testing.png", &texture);
  
  geo__rect_t source = {0, 0, 32, 32};
  geo__rect_t dest = {0, 0, 32, 32};

  struct font__handle_t* font = NULL;
  fprintf( stderr, "font rc: %d\n",
	   font__create("resources/font/test_font.dat", &font) );

  int x_value_1 = 175;
  int y_value_1 = 250;

  int x_value_2 = 225;
  int y_value_2 = 250;

  while ( keep_looping ) {

    timing__declare_top_of_frame();

    events__process_events();

    video__clearscreen();

    for ( dest.x = 0; dest.x < 400; dest.x += 32 ){
      for ( dest.y = 0; dest.y < 300; dest.y += 32 ){
    	video__blit(texture, &source, &dest);
      }
    }

    const struct control__state_t* control_1 = control__get_state(0);
    
    x_value_1 -= (int)(control_1->left.value*10);
    x_value_1 += (int)(control_1->right.value*10);

    y_value_1 -= (int)(control_1->up.value*10);
    y_value_1 += (int)(control_1->down.value*10);

    dest.x = x_value_1;
    dest.y = y_value_1;
    video__rect( &dest, 0, 0, 255, 255 );

    const struct control__state_t* control_2 = control__get_state(1);
    
    x_value_2 -= (int)(control_2->left.value*10);
    x_value_2 += (int)(control_2->right.value*10);

    y_value_2 -= (int)(control_2->up.value*10);
    y_value_2 += (int)(control_2->down.value*10);

    dest.x = x_value_2;
    dest.y = y_value_2;
    video__rect( &dest, 255, 0, 0, 255 );

    geo__line_t line = { 30, 30, 350, 45 };

    video__line( &line, 0, 255, 0, 192 );

    
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
