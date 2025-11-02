#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  pid_t child_pid = fork();
  assert(child_pid != -1);
  if (child_pid > 0)
    return EXIT_SUCCESS;
  setsid();
  child_pid = fork();
  assert(child_pid != -1);
  if (child_pid > 0)
    return EXIT_SUCCESS;

  pid_t pid = getpid();
  int pid_fd = open("screamd.pid", O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, 0644);
  assert(pid_fd != -1);
  FILE *pid_file = fdopen(pid_fd, "w");
  assert(pid_file != NULL);
  fprintf(pid_file, "%d", pid);
  fclose(pid_file);

  const char *log_file_path = "screamd.log";
  FILE *log_file = fopen(log_file_path, "a");
  assert(log_file != NULL);
  setlinebuf(log_file);
  setlinebuf(stdin);
  setlinebuf(stdout);
  int log_fd = fileno(log_file);
  dup2(log_fd, STDOUT_FILENO);
  dup2(log_fd, STDERR_FILENO);

  while (1) {
    time_t now = time(NULL);
    char *t = ctime(&now);
    fputs(t, stdout);
    sleep(1);
  }

  return EXIT_SUCCESS;
}
