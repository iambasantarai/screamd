#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 128
#define MIN_THRESHOLD_MIB 1024

int main(void) {
  char *mem_info_cmd = "free -m | grep 'Mem:' | awk '{print $4}'";
  int mem_avail = 0;

  char mem_info_buf[BUFSIZE] = {0};
  FILE *fp;

  if ((fp = popen(mem_info_cmd, "r")) == NULL) {
    printf("Error opening pipe!\n");
    return EXIT_FAILURE;
  }

  while (fgets(mem_info_buf, BUFSIZE, fp) != NULL) {
    mem_avail = atoi(mem_info_buf);
    // NOTE: reverse comparison operator
    if (mem_avail > MIN_THRESHOLD_MIB) {
      system("aplay scream.wav");
    }
  }

  if (pclose(fp)) {
    printf("Command not found or exited with error status\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
