#include <video.h>
#include <events.h>
#include <constants.h>

#include <unistd.h>

void on_quit( void* context ) {
  *((int*)context) = FALSE;
}

int main( int argc, char** argv ) {

  video__setup();

  int keep_looping = TRUE;

  events__set_callback( EVENTS__ON_QUIT, on_quit, &keep_looping );

  while ( keep_looping ) {
    events__process_events();
  }

  video__teardown();

  return 0;
}
