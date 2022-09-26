#include <stdint.h>
#include <stdlib.h>
#include "../common.h"
#include "../array/array.h"

#ifndef LIB_STRUCTURES_STACK_H
#define LIB_STRUCTURES_STACK_H

typedef struct stack_struct {
  array_t* array;
} stack_t;

enum ss_status stack_create(size_t capacity, stack_t** out);
void stack_destroy(stack_t** pointer);

size_t stack_length(stack_t* stack);
size_t stack_capacity(stack_t* stack);

enum ss_status stack_peek(stack_t* stack, void** out);
enum ss_status stack_push(stack_t* stack, void* pointer);
enum ss_status stack_pop(stack_t* stack, void** out);

#endif

