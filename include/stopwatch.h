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

static
void stopwatch__init( struct stopwatch_t* sw )
{
  sw->start.tv_sec = 0;
  sw->start.tv_usec = 0;

  sw->stop.tv_sec = 0;
  sw->stop.tv_usec = 0;

  sw->total.tv_sec = 0;
  sw->total.tv_usec = 0;

  sw->longest.tv_sec = 0;
  sw->longest.tv_usec = 0;

  sw->shortest.tv_sec = 0x7FFFFFFF;
  sw->shortest.tv_usec = 0x7FFFFFFF;

  sw->invocations = 0;
}

#ifdef STOPWATCH_ENABLED

static
void stopwatch__start( struct stopwatch_t* sw ){
  gettimeofday( &(sw->start), NULL );
}

static
void stopwatch__stop( struct stopwatch_t* sw ){

  struct timeval diff;
  
  gettimeofday( &(sw->stop), NULL );

  diff.tv_sec = sw->stop.tv_sec - sw->start.tv_sec;
  diff.tv_usec = sw->stop.tv_usec - sw->start.tv_usec;
  if ( diff.tv_usec < 0 ){
    diff.tv_sec -= 1;
    diff.tv_usec += 1000000;
  }

  sw->total.tv_sec += diff.tv_sec;
  sw->total.tv_usec += diff.tv_usec;
  if ( 1000000 < sw->total.tv_usec ){
    diff.tv_sec += 1;
    diff.tv_usec -= 1000000;
  }

  if ((sw->longest.tv_sec < diff.tv_usec) ||
      ((sw->longest.tv_sec == diff.tv_usec) &&
       (sw->longest.tv_usec < diff.tv_usec))){
    sw->longest = diff;
  }

  if ((diff.tv_sec < sw->shortest.tv_usec) ||
      ((diff.tv_sec == sw->shortest.tv_usec) &&
       (diff.tv_usec < sw->shortest.tv_usec))){
    sw->shortest = diff;
  }

  ++(sw->invocations);
}

static
void stopwatch__dump( struct stopwatch_t* sw, const char* name, FILE* fout )
{
  struct timeval mean;
  mean.tv_sec = sw->total.tv_sec / sw->invocations;
  mean.tv_usec = sw->total.tv_usec / sw->invocations;

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

static
void stopwatch__start( struct stopwatch_t* sw ) { }

static
void stopwatch__stop( struct stopwatch_t* sw ) { }

static
void stopwatch__dump( struct stopwatch_t* sw, const char* name, FILE* fout )
{
  fprintf( fout, "%s: STOPWATCHES NOT ENABLED\n", name );
}

#endif

#endif
