#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct {
  int x;
  int y;

  int width;
  int height;
} geo__rect_t;

typedef struct {
  int x;
  int y;
} geo__point_t;

typedef struct {
  int x1;
  int y1;

  int x2;
  int y2;
} geo__line_t;

#endif