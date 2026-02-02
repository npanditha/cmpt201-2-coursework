// Nisal Panditha
// CMPT 201
// lab3

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 5

static void free_history(char *history[HISTORY_SIZE]) {
  for (int i = 0; i < HISTORY_SIZE; i++) {
    free(history[i]);
    history[i] = NULL;
  }
}

static void add_to_history(char *history[HISTORY_SIZE], int *head, int *count,
                           char *line) {
  // If overwriting an existing entry, free it first
  if (history[*head] != NULL) {
    free(history[*head]);
    history[*head] = NULL;
  }

  history[*head] = line;              // take ownership of the allocated line
  *head = (*head + 1) % HISTORY_SIZE; // advance circular index

  if (*count < HISTORY_SIZE) {
    (*count)++;
  }
}

static void print_history(char *history[HISTORY_SIZE], int head, int count) {
  // Oldest entry is at (head - count) modulo HISTORY_SIZE
  int start = (head - count + HISTORY_SIZE) % HISTORY_SIZE;

  for (int i = 0; i < count; i++) {
    int idx = (start + i) % HISTORY_SIZE;
    if (history[idx] != NULL) {
      fputs(history[idx],
            stdout); // history lines include '\n' (even blank lines are "\n")
    }
  }
}

static int is_print_command(const char *line) {
  // Accept "print\n" or "print" (if last line doesn't include newline)
  if (strcmp(line, "print\n") == 0)
    return 1;
  if (strcmp(line, "print") == 0)
    return 1;
  return 0;
}

int main(void) {
  char *history[HISTORY_SIZE] = {0};
  int head = 0;  // next slot to write
  int count = 0; // number of valid stored lines (<= HISTORY_SIZE)

  while (1) {
    printf("Enter input: ");
    fflush(stdout);

    char *line = NULL;
    size_t cap = 0;

    ssize_t nread = getline(&line, &cap, stdin);
    if (nread == -1) {
      // EOF (Ctrl+D) or error
      free(line);
      break;
    }

    add_to_history(history, &head, &count, line);

    if (is_print_command(line)) {
      print_history(history, head, count);
    }
  }

  free_history(history);
  return 0;
}
