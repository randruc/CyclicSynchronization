/**
 * Remi Andruccioli
 * February 2019
 *
 * Here is an implementation with 2 Unix processes instead of 2 threads.
 * The difference is that 2 processes have distinct memory areas by default.
 * So as one of the many possible implementations, I use Unix pipes as a way of
 * signaling events.
 * In each loop the PID of the process is printed followed by the letter A or B.
 *
 * To compile:
 * gcc -Wall -Wextra -Werror -pedantic -std=c89 cyclic_sync_processes.c
 *
 * This source code is under public domain.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

static const int MAXLOOP = 5;

int
main()
{
  int pipeA[2];
  int pipeB[2];
  pid_t childPid;
  int i;
  char c;

  if ((pipe(pipeA) < 0) || (pipe(pipeB) < 0)) {
    perror("pipe");

    exit(EXIT_FAILURE);
  }

  if ((childPid = fork()) < 0) {
    perror("fork");

    exit(EXIT_FAILURE);
  }

  if (0 == childPid) {
    /* Child process: A */
    for (i = 0; i < MAXLOOP; ++i) {
      read(pipeA[0], &c, 1);
      fprintf(stderr, "%ld A\n", (long)getpid());
      write(pipeB[1], &c, 1);
    }
  } else {
    /* Parent process: B */
    write(pipeA[1], &c, 1);

    for (i = 0; i < MAXLOOP; ++i) {
      read(pipeB[0], &c, 1);
      fprintf(stderr, "%ld B\n", (long)getpid());
      write(pipeA[1], &c, 1);
    }
  }

  return EXIT_SUCCESS;
}

