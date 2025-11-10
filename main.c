#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 256
#define MIN_MEM_THRES_IN_MIB 1024

void scream(void) {
  char *cmd = "aplay -D sysdefault scream.wav";

  int result = system(cmd);
  if (result != 0) {
    fprintf(stderr, "Error playing sound: system() returned %d\n", result);
  }
}

int get_available_memory(void) {
    FILE *meminfo = fopen("/proc/meminfo", "r");
    if (meminfo == NULL) {
        printf("Error reading file.\n");
        return EXIT_FAILURE;
    }

    char line[BUF_SIZE];
    while (fgets(line, sizeof(line), meminfo)) {
        int avail_mem;
        if (sscanf(line, "MemAvailable: %d kB", &avail_mem) == 1) {
            fclose(meminfo);
            return avail_mem;
        }
    }

    fclose(meminfo);
    return EXIT_FAILURE;
}

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

  printf("Starting daemon...\n");
  while (1) {
    int avail_mem = get_available_memory();
    int avail_mem_in_mib = avail_mem/1024;

    if (avail_mem_in_mib < MIN_MEM_THRES_IN_MIB) {
      scream();
    }

    sleep(60);
  }

  return EXIT_SUCCESS;
}
