#ifndef UTILS_H
#define UTILS_H


int utils__sqrt( int i );

int utils__pos2screen( int pos );

int utils__screen2pos( int screen );

int utils__round( float );

int utils__clamp( int min, int max, int value );

float utils__clampf( float min, float max, float value );

#endif
