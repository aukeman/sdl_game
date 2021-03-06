#include <utils.h>
#include <math.h>

int utils__sqrt( int i ){

  float result = sqrt(i);

  /* handle NANs */
  if ( result != result ){
    return 0;
  }
  else{
    return utils__round(result);
  }
}

int utils__pos2screen( int pos )
{
  return (pos >> 10);
}

int utils__screen2pos( int screen )
{
  return (screen << 10);
}

int utils__round( float input ){
  if ( input < 0.0f ){
    return (int)(input - 0.5f);
  }
  else{
    return (int)(input + 0.5f);
  }
}

int utils__clamp( int min, int max, int value ){

  if ( value <= min ){
    return min;
  }
  else if ( max <= value ){
    return max;
  }
  else{
    return value;
  }
}

float utils__clampf( float min, float max, float value ){

  if ( value <= min ){
    return min;
  }
  else if ( max <= value ){
    return max;
  }
  else{
    return value;
  }
}
