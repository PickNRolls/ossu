#include "../common.h"
#include "../array/array.h"
#include "stack.h"
#include <stdlib.h>

enum ss_status stack_create(size_t capacity, stack_t** out) {
  stack_t* stack = malloc(sizeof(stack_t));
  if (!stack) {
    return SS_ERR_ALLOC;
  }

  array_t* array = 0;
  enum ss_status status = 0;
  if ((status = array_create(capacity, &array)) == SS_OK) {
    stack->array = array;
  } else {
    free(stack);
    return status;
  }

  *out = stack;
  return SS_OK;
}

void stack_destroy(stack_t** pointer) {
  if (!pointer) {
    return;
  }

  stack_t* stack = *pointer;
  if (!stack) {
    return;
  }

  *pointer = 0;
  array_destroy(&stack->array);
  free(stack);
}

size_t stack_length(stack_t* stack) {
  return array_length(stack->array);
}

size_t stack_capacity(stack_t* stack) {
  return array_capacity(stack->array);
}

enum ss_status stack_peek(stack_t* stack, void** out) {
  return array_get_last(stack->array, out);
}

enum ss_status stack_push(stack_t* stack, void* pointer) {
  return array_add_last(stack->array, pointer);
}

enum ss_status stack_pop(stack_t* stack, void** out) {
  return array_remove_last(stack->array, out);
}

enum ss_status stack_resize(stack_t** pointer, size_t capacity) {
  return array_resize(&((*pointer)->array), capacity);
}

