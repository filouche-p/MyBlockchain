#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>




int main(void) {
    printf("%d\n", str_to_int("1245"));
    printf("%d\n", str_to_int("-1245"));
    return 0;
}