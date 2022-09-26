#include <stdint.h>
#include <stdlib.h>
#include "../common.h"

#ifndef LIB_STRUCTURES_ARRAY_H
#define LIB_STRUCTURES_ARRAY_H

typedef struct array_struct {
  size_t length;
  size_t capacity;
  void* buffer[];
} array_t;

enum ss_status array_create(size_t capacity, array_t** out);
void array_destroy(array_t** pointer);

size_t array_length(array_t* array);
size_t array_capacity(array_t* array);

enum ss_status array_get_at(array_t* array, size_t index, void** out);
enum ss_status array_get_last(array_t* array, void** out);
enum ss_status array_get_first(array_t* array, void** out);

enum ss_status array_replace_at(array_t* array, size_t index, void* pointer);
enum ss_status array_replace_last(array_t* array, void* pointer);
enum ss_status array_add_at(array_t* array, size_t index, void* pointer);
enum ss_status array_add_last(array_t* array, void* pointer);
enum ss_status array_remove_at(array_t* array, size_t index, void** out);
enum ss_status array_remove_last(array_t* array, void** out);

enum ss_status array_resize(array_t** pointer, size_t capacity);

#endif

