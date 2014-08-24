#ifndef BACKGROUND_H
#define BACKGROUND_H

struct entity_t;

struct background_t{
  geo__point_t position;
  const struct entity_t* prototype;
};

#endif
