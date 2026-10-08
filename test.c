#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int main(void) {
    char c = 'a';

    write(stdout, c, 1);

    return 0;
}