#include <utils.h>
#include <math.h>

int utils__sqrt( int i ){
  float result = rintf( sqrt(i) );

  /* handle NANs */
  return result == result ? (int)result : 0;
}
