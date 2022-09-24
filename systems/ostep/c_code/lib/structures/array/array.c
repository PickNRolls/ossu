#include "../common.h"
#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum ss_status array_create(size_t capacity, array_t** out) {
  if (capacity < 1) {
    return SS_ERR_INVALID_CAPACITY;
  }

  array_t* array = malloc(sizeof(array_t) + capacity * sizeof(void*));
  if (array == 0) {
    return SS_ERR_ALLOC;
  }
  array->capacity = capacity;
  array->length = 0;
  *out = array;
  return SS_OK;
}

void array_destroy(array_t** p) {
  if (!p) {
    return;
  }

  array_t* array = *p;
  if (!array) {
    return;
  }

  *p = 0;
  free(array);
}

size_t array_length(array_t* array) {
  return array->length;
}

size_t array_capacity(array_t* array) {
  return array->capacity;
}

enum ss_status array_get_at(array_t* array, size_t index, void** out) {
  size_t length = array_length(array);
  if (index < 0 || index >= length) {
    return SS_ERR_OUT_OF_RANGE;
  }

  *out = array->buffer[index];
  return SS_OK;
}

enum ss_status array_get_last(array_t* array, void** out) {
  return array_get_at(array, array_length(array) - 1, out);
}

enum ss_status array_get_first(array_t* array, void** out) {
  return array_get_at(array, 0, out);
}

enum ss_status array_replace_at(array_t* array, size_t index, void* pointer) {
  size_t length = array_length(array);
  if (index < 0 || (long)index > (long)length - 1) {
    return SS_ERR_OUT_OF_RANGE;
  }

  array->buffer[index] = pointer;
  return SS_OK;
}

enum ss_status array_replace_last(array_t* array, void* pointer) {
  return array_replace_at(array, array_length(array) - 1, pointer);
}

enum ss_status array_add_at(array_t* array, size_t index, void* pointer) {
  size_t capacity = array_capacity(array);
  size_t length = array_length(array);
  if (length == capacity) {
    return SS_ERR_MAX_CAPACITY;
  }

  if (index != length) {
    size_t shift = (length - index) * sizeof(void*);
    memmove(&array->buffer[index + 1], &array->buffer[index], shift);
  }
  array->buffer[index] = pointer;
  array->length++;

  return SS_OK;
}

enum ss_status array_add_last(array_t* array, void* pointer) {
  return array_add_at(array, array_length(array), pointer);
}

enum ss_status array_remove_at(array_t* array, size_t index, void** out) {
  size_t length = array_length(array);
  if (index < 0 || index >= length) {
    return SS_ERR_OUT_OF_RANGE;
  }

  *out = array->buffer[index];
  if (index != length - 1) {
    size_t shift = (length - 1 - index) * sizeof(void*);
    memmove(&array->buffer[index], &array->buffer[index + 1], shift);
  }

  array->length--;

  return SS_OK;
}

enum ss_status array_remove_last(array_t* array, void** out) {
  return array_remove_at(array, array_length(array) - 1, out);
}

enum ss_status array_resize(array_t** pointer, size_t capacity) {
  array_t* array = *pointer;
  if (capacity == array_capacity(array)) {
    return SS_OK;
  }
  if (capacity < array_length(array)) {
    return SS_ERR_INVALID_CAPACITY;
  }

  array = realloc(array, capacity);
  if (!array) {
    return SS_ERR_ALLOC;
  }
  *pointer = array;
  array->capacity = capacity;

  return SS_OK;
}

