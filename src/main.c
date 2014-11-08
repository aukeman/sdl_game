#include <video.h>
#include <events.h>
#include <constants.h>
#include <timing.h>
#include <font.h>
#include <control.h>
#include <player.h>
#include <background.h>
#include <stopwatch.h>

#include <unistd.h>
#include <stdio.h>
#include <math.h>

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
  video__setup_texture("resources/img/background.png", &texture);
  
  struct font__handle_t* font = NULL;
  font__create("resources/font/test_font.dat", &font);

  struct background_t* background = NULL;
  background__create("resources/background/background.dat", &background);

  struct player_prototype_t default_player = { &player__basic_draw,
					       &player__basic_update,
					       NULL };

  struct player_t players[2] = {
    { { 175, 225 }, { 0, 0 }, &default_player, control__get_state(0), { 255, 0,   0 } },
    { { 225, 225 }, { 0, 0 }, &default_player, control__get_state(1), {   0, 0, 255 } }
  };

  struct stopwatch_t process_events_sw, draw_bg_sw, draw_players_sw, update_players_sw, draw_stats_sw, flip_page_sw, frame_sw;
  stopwatch__init(&process_events_sw);
  stopwatch__init(&draw_bg_sw);
  stopwatch__init(&draw_players_sw);
  stopwatch__init(&update_players_sw);
  stopwatch__init(&draw_stats_sw);
  stopwatch__init(&flip_page_sw);
  stopwatch__init(&frame_sw);

  video__clearscreen();

  uint32_t pos_x = 0;
  uint32_t pos_y = 0;

  while ( keep_looping ) {

    stopwatch__start(&frame_sw);

    timing__declare_top_of_frame();

    milliseconds_t frame_length = timing__get_frame_length();

    stopwatch__start(&process_events_sw);
    events__process_events();

    if ( control__get_state(1)->up.value ){
      pos_y -= 10;
    }

    if ( control__get_state(1)->down.value ){
      pos_y += 10;
    }

    if ( control__get_state(1)->left.value ){
      pos_x -= 10;
    }

    if ( control__get_state(1)->right.value ){
      pos_x += 10;
    }
    stopwatch__stop(&process_events_sw);

    stopwatch__start(&draw_bg_sw);
    background__draw(pos_x, pos_y, background);
    stopwatch__stop(&draw_bg_sw);

    int player_idx = 0;
    for ( player_idx = 0; player_idx < 2; ++player_idx ){
      stopwatch__start(&draw_players_sw);
      players[player_idx].prototype->draw_fxn( &players[player_idx] );
      stopwatch__stop(&draw_players_sw);
      
      stopwatch__start(&update_players_sw);
      players[player_idx].prototype->update_fxn( &players[player_idx], frame_length );
      stopwatch__stop(&update_players_sw);
    }

    stopwatch__start(&draw_stats_sw);
    font__draw_string(font, 0, 0,
    		      "FPS:         %5.1f\n"
    		      "Frame Count: %5d\n"
    		      "Fame Length: %5d",
    		      timing__get_instantaneous_fps(),
    		      timing__get_frame_count(),
    		      timing__get_frame_length());
    stopwatch__stop(&draw_stats_sw);

    stopwatch__start(&flip_page_sw);
    video__flip();
    stopwatch__stop(&flip_page_sw);

    stopwatch__stop(&frame_sw);
  }

  fprintf( stdout, 
	   "%d frames; %f seconds %f fps average %f fps instantaneous\n",
	   timing__get_frame_count(),
	   timing__get_top_of_frame() / 1000.0f,
	   timing__get_average_fps(),
	   timing__get_instantaneous_fps());

  stopwatch__dump(&process_events_sw, "Process Events", stdout);
  stopwatch__dump(&draw_bg_sw, "Draw Background", stdout);
  stopwatch__dump(&draw_players_sw, "Draw Players", stdout);
  stopwatch__dump(&update_players_sw, "Update Players", stdout);
  stopwatch__dump(&draw_stats_sw, "Draw Stats", stdout);
  stopwatch__dump(&flip_page_sw, "Flip Page", stdout);
  stopwatch__dump(&frame_sw, "Frame", stdout);

  background__free(background);
	   
  font__free(font);
  
  timing__teardown();
  js__teardown();
  video__teardown();

  return 0;
}
