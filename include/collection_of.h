#ifndef COLLECTION_OF_H
#define COLLECTION_OF_H

#include <string.h>

#include <types.h>
#include <constants.h>

#define COLLECTION_OF(type, size) __collection_of_ ## size ## _ ## type ## s_t
#define DECLARE_COLLECTION_OF(type, size) \
  typedef struct {	                  \
    type values[size];                    \
    type* sentinel;                 \
  } COLLECTION_OF(type, size)

#define INIT_COLLECTION_OF(type, size, collection) \
  do { \
    memset( (collection), '\0', sizeof( COLLECTION_OF(type, size)));	\
    (collection)->sentinel = (collection)->values; \
  }while(0)

#define ADD_TO_COLLECTION(collection, item) \
  ( (collection)->sentinel < (collection)->values + sizeof((collection)->values)/sizeof((collection)->values[0]) ? ((*((collection)->sentinel) = item), ++(collection)->sentinel ? TRUE : TRUE)  : FALSE )

#endif
