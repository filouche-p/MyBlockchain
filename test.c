#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

int main(void) {
    char *string = malloc(5);

    string[4] = '\0';

    free(string);

    return 0;
}