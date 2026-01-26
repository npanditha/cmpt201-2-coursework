// Nisal Panditha
// CMPT 201 Lab2

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char *line = NULL;
  size_t cap = 0;

  while (1) {
    printf("Enter programs to run.\n> ");
    fflush(stdout);

    ssize_t nread = getline(&line, &cap, stdin);
    if (nread == -1) {
      // EOF or error (Ctrl+D). Lab wants Ctrl+C to end, but this is fine.
      putchar('\n');
      break;
    }

    // Remove trailing newline (linefeed)
    if (nread > 0 && line[nread - 1] == '\n') {
      line[nread - 1] = '\0';
    }

    // Ignore empty input
    if (line[0] == '\0') {
      continue;
    }

    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      continue;
    }

    if (pid == 0) {
      // Child: run the program. argv[0] should be the program name/path.
      execl(line, line, (char *)NULL);

      // If execl returns, it failed.
      printf("Exec failure\n");
      fflush(stdout);
      _exit(127);
    } else {
      // Parent: wait for the child to finish
      int status = 0;
      if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
      }
    }
  }

  free(line);
  return 0;
}
