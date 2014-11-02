#include <video.h>
#include <events.h>
#include <constants.h>
#include <timing.h>
#include <font.h>
#include <control.h>
#include <player.h>
#include <background.h>
#include <stopwatch.h>

#include <update_functions.h>

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

  struct background_prototype_t backgrounds[] = {
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 0, 0 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 0, 1 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 1, 0 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 1, 1 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 2, 0 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 2, 1 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 3, 0 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 3, 1 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 0, 2 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 0, 3 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 1, 2 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 1, 3 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 2, 2 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 2, 3 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 3, 2 },
    { &background__basic_draw, texture, BACKGROUND__COLLISION_NONE, 3, 3 },
  };

  const int number_of_columns = 400/BACKGROUND_TILE_WIDTH+1;
  const int number_of_rows = 300/BACKGROUND_TILE_HEIGHT+1;

  struct background_t background[number_of_columns][number_of_rows];

  int col_idx, row_idx;
  for ( col_idx = 0; col_idx < number_of_columns; ++col_idx ){
    for ( row_idx = 0; row_idx < number_of_rows; ++row_idx ){

      size_t prototype_idx_base;

      switch ( (col_idx >> 2) % 4 ){
      case 0: prototype_idx_base =  0; break;
      case 1: prototype_idx_base =  4; break;
      case 2: prototype_idx_base =  8; break;
      case 3: prototype_idx_base = 12; break;
      }

      size_t prototype_idx =  2*(col_idx % 2) + (row_idx % 2);

      background[col_idx][row_idx].prototype = &backgrounds[prototype_idx_base + prototype_idx];
    }
  }

  struct player_prototype_t default_player = { &player__basic_draw,
					       &player__basic_update,
					       NULL };

  struct player_t players[2] = {
    { { 175, 225 }, { 0, 0 }, &default_player, control__get_state(0), { 255, 0,   0 } },
    { { 225, 225 }, { 0, 0 }, &default_player, control__get_state(1), {   0, 0, 255 } }
  };

  struct stopwatch_t process_events_sw, draw_bg_sw, draw_players_sw, draw_stats_sw, flip_page_sw;
  stopwatch__init(&process_events_sw);
  stopwatch__init(&draw_bg_sw);
  stopwatch__init(&draw_players_sw);
  stopwatch__init(&draw_stats_sw);
  stopwatch__init(&flip_page_sw);

  while ( keep_looping ) {

    timing__declare_top_of_frame();

    milliseconds_t frame_length = timing__get_frame_length();

    stopwatch__start(&process_events_sw);
    events__process_events();
    stopwatch__stop(&process_events_sw);

    stopwatch__start(&draw_bg_sw);
    video__begin_blits(NULL);
    for ( col_idx = 0; col_idx < number_of_columns; ++col_idx ){
      for ( row_idx = 0; row_idx < number_of_rows; ++row_idx ){
    	background[col_idx][row_idx].prototype->draw_fxn( col_idx,
    							  row_idx,
    							  &background[col_idx][row_idx] );
      }
    }
    video__end_blits();
    stopwatch__stop(&draw_bg_sw);

    stopwatch__start(&draw_players_sw);
    int player_idx = 0;
    for ( player_idx = 0; player_idx < 2; ++player_idx ){
      players[player_idx].prototype->draw_fxn( &players[player_idx] );
      players[player_idx].prototype->update_fxn( &players[player_idx], frame_length );
    }
    stopwatch__stop(&draw_players_sw);

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
  stopwatch__dump(&draw_stats_sw, "Draw Stats", stdout);
  stopwatch__dump(&flip_page_sw, "Flip Page", stdout);
	   
  font__free(font);
  
  timing__teardown();
  js__teardown();
  video__teardown();

  return 0;
}
