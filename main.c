#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("screamd: daemon to make pc scream when memory < 1024MiB\n");

    int status = system("aplay scream.wav1");
    if(status != 0){
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
