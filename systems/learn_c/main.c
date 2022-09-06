#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  printf("Hello world\n");

  size_t* count = malloc(sizeof(size_t));
  (*count) = 0;
  (*count)++;
  (*count)++;
  (*count)++;

  printf("%lu\n", *count);

  return 0;
}
