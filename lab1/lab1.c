
// Nisal Panditha
// CMPT 201 Lab 1

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *buffer = NULL;
  size_t buffer_size = 0;
  ssize_t bytes_read;

  while (1) {
    printf("Please enter some text: ");
    fflush(stdout);

    bytes_read = getline(&buffer, &buffer_size, stdin);

    if (bytes_read == -1) {
      /* EOF (Ctrl+D) or error */
      if (feof(stdin)) {
        break; // normal exit
      } else {
        perror("getline");
        free(buffer);
        exit(EXIT_FAILURE);
      }
    }

    /* Remove trailing newline if present */
    if (bytes_read > 0 && buffer[bytes_read - 1] == '\n') {
      buffer[bytes_read - 1] = '\0';
    }

    printf("Tokens:\n");

    char *saveptr;
    char *token = strtok_r(buffer, " ", &saveptr);

    while (token != NULL) {
      printf("  %s\n", token);
      token = strtok_r(NULL, " ", &saveptr);
    }
  }

  free(buffer);
  return 0;
}
