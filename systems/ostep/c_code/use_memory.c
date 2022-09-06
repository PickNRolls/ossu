#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void safe_gettimeofday(struct timeval* tv) {
  if (gettimeofday(tv, NULL)) {
    fprintf(stderr, "failed to get time of day\n");
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr,
            "usage: use_memory <int: mb amount> <int: duration in seconds>\n");
    exit(1);
  }

  struct timeval prev_time = {0};
  safe_gettimeofday(&prev_time);
  struct timeval current_time = {0};
  safe_gettimeofday(&current_time);

  size_t megabytes_to_use = atoi(argv[1]);
  size_t array_size = sizeof(char) * 1024 * 1024 * megabytes_to_use;

  printf("%li megabytes allocated\n", megabytes_to_use);

  char* array = malloc(array_size);
  if (array == NULL) {
    fprintf(stderr, "failed to allocate\n");
    exit(0);
  }
  size_t remaining_seconds = atol(argv[2]);
  while (remaining_seconds > 0) {
    prev_time = current_time;
    safe_gettimeofday(&current_time);

    if (prev_time.tv_sec != current_time.tv_sec) {
      remaining_seconds--;
    }

    memset(array, 'w', array_size);
  }

  free(array);
  return 0;
}
