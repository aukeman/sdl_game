#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>

struct stopwatch_t
{
  struct timeval start;
  struct timeval stop;

  struct timeval total;
  struct timeval longest;
  struct timeval shortest;

  uint32_t invocations;
};

static inline
void stopwatch__init( struct stopwatch_t* sw )
{
  timerclear( &(sw->start) );
  timerclear( &(sw->stop) );

  timerclear( &(sw->total) );
  timerclear( &(sw->longest) );
  timerclear( &(sw->shortest) );

  sw->invocations = 0;
}

#ifdef STOPWATCH_ENABLED

static inline
void stopwatch__start( struct stopwatch_t* sw ){
  gettimeofday( &(sw->start), NULL );
}

static inline
void stopwatch__stop( struct stopwatch_t* sw ){

  gettimeofday( &(sw->stop), NULL );

  struct timeval diff, temp;
  
  timersub( &(sw->stop), &(sw->start), &diff );
  timeradd( &diff, &(sw->total), &temp );
  sw->total = temp;

  if ( timercmp( &(sw->longest), &diff, < ) ) {
    sw->longest = diff;
  }

  if ( !timerisset( &(sw->shortest) ) ||
       timercmp( &diff, &(sw->shortest), < ) ){
    sw->shortest = diff;
  }

  ++(sw->invocations);
}

static inline
void stopwatch__dump( struct stopwatch_t* sw, const char* name, FILE* fout )
{
  struct timeval mean = { sw->total.tv_sec / sw->invocations,
			  sw->total.tv_usec / sw->invocations }; 

  if ( sw->total.tv_sec % sw->invocations )
  {
    mean.tv_usec += 
      1000000 * (sw->total.tv_sec % sw->invocations) / sw->invocations;
  }


  fprintf( fout, "%s: tot: %ld.%06ld  avg:  %ld.%06ld  lng: %ld.%06ld  sht: %ld.%06ld  #: %d\n",
	   name,
	   (long int)(sw->total.tv_sec),
	   (long int)(sw->total.tv_usec),
	   (long int)(mean.tv_sec),
	   (long int)(mean.tv_usec),
	   (long int)(sw->longest.tv_sec),
	   (long int)(sw->longest.tv_usec),
	   (long int)(sw->shortest.tv_sec),
	   (long int)(sw->shortest.tv_usec),
	   sw->invocations );
}

#else

static inline
void stopwatch__start( struct stopwatch_t* sw ) { }

static inline
void stopwatch__stop( struct stopwatch_t* sw ) { }

static inline
void stopwatch__dump( struct stopwatch_t* sw, const char* name, FILE* fout )
{
  fprintf( fout, "%s: STOPWATCHES NOT ENABLED\n", name );
}

#endif

#endif
