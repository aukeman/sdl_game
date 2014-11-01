#ifndef UTILS_H
#define UTILS_H


int utils__sqrt( int i );

static inline 
int utils__pos2screen( int pos )
{
  return (pos >> 2);
}

static inline
int utils__screen2pos( int screen )
{
  return (screen << 2);
}

#endif
