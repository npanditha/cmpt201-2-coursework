// Nisal Panditha
// CMPT 201
// Lab4

#define _DEFAULT_SOURCE
#include <inttypes.h> // PRIu64
#include <stdint.h>   // uint64_t
#include <stdio.h>    // snprintf
#include <string.h>   // memset
#include <unistd.h>   // sbrk, write

#define EXTRA_SIZE 256
#define BLOCK_SIZE 128
#define BUF_SIZE 128

struct header {
  uint64_t size;
  struct header *next;
};

static void print_out(const char *fmt, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  int n;
  if (data_size == sizeof(uint64_t)) {
    n = snprintf(buf, sizeof(buf), fmt, *(uint64_t *)data);
  } else {
    n = snprintf(buf, sizeof(buf), fmt, *(void **)data);
  }
  if (n > 0)
    (void)write(STDOUT_FILENO, buf, (size_t)n);
}

static void print_byte(unsigned char b) {
  char line[2] = {(char)('0' + (b & 1u)), '\n'};
  (void)write(STDOUT_FILENO, line, 2);
}

int main(void) {

  void *start = sbrk(EXTRA_SIZE);
  if (start == (void *)-1)
    return 1;

  struct header *first = (struct header *)start;
  struct header *second = (struct header *)((char *)start + BLOCK_SIZE);

  const size_t header_sz = sizeof(struct header);
  const size_t data_sz = BLOCK_SIZE - header_sz;

  first->size = BLOCK_SIZE;
  first->next = NULL;
  second->size = BLOCK_SIZE;
  second->next = first;

  unsigned char *first_data = (unsigned char *)(first + 1);
  unsigned char *second_data = (unsigned char *)(second + 1);
  memset(first_data, 0x00, data_sz);
  memset(second_data, 0x01, data_sz);

  print_out("first block:       %p\n", &first, sizeof(first));
  print_out("second block:      %p\n", &second, sizeof(second));

  print_out("first block size:  %" PRIu64 "\n", &first->size, sizeof(uint64_t));
  print_out("first block next:  %p\n", &first->next, sizeof(first->next));

  print_out("second block size: %" PRIu64 "\n", &second->size,
            sizeof(uint64_t));
  print_out("second block next: %p\n", &second->next, sizeof(second->next));

  for (size_t i = 0; i < data_sz; ++i)
    print_byte(first_data[i]);
  for (size_t i = 0; i < data_sz; ++i)
    print_byte(second_data[i]);

  return 0;
}
