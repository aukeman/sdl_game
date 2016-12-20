#include <utils.h>
#include <math.h>

int utils__sqrt( int i ){
  float result = rintf( sqrt(i) );

  /* handle NANs */
  return result == result ? (int)result : 0;
}

int utils__pos2screen( int pos )
{
  return (pos >> 2);
}

int utils__screen2pos( int screen )
{
  return (screen << 2);
}
