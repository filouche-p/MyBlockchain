#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "./libs/headers/utils.h"
#include "./libs/headers/command_handler.h"


//malloc, free, printf, write, open, read, close


// chemin d'accès dynamique ?
int save_blockchain() { 
    char *filename = ".blockchain.save";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 644);

    if(fd == -1) {
        print_error(1);
        return 0;
    }

    return 1;
}

char *read_blockchain() {
    char *filename = ".bloackchain.save";
    int fd = open(filename, O_RDONLY);
    if(fd == -1) {
        // throw error
        printf("Error while reading\n");
        return 0;
    }

    size_t size = get_file_size(filename);
    if (size == 0) {
        return "\0";
    }

    char *data = smalloc(size);
    read(fd, &data, size);

    close(fd);

    return data;
}


// int rtos(char *raw_data) {
    // raw to struct
    // chaque ligne est un node
    // le nid est les premiers caractères de chaque ligne jusqu'au :
    // les : peuvent être ignorer
    // chaque blockid sont séparer par des , (potentiellement des espaces)

    // char **rows = split(raw_data, '\n');

    // return 0;
// }

int main(void) {
    size_t i = 0;
    while(i < 50) {
        char sync_state = 's';
        int n_node = 0;

        printf("[%c%d", sync_state, n_node);

        char *res = input("]> ", 100);

        if(my_strcmp(res, "quit") == 0) {
            free(res);
            break;
        }

        if(!res) {
            printf("\n");
        }
        
        check_command(res);

        free(res);

        i++;
    }

    return 0;
}