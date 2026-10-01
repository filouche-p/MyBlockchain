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
#include "./libs/headers/handler.h"


//malloc, free, printf, write, open, read, close
typedef struct s_block {
    char *bid;
    struct s_block *next;
} t_block;

typedef struct s_node {
    int nid;
    t_block *first_block;
    struct s_node *next;
} t_node;

bool check_command(char *command) {
    size_t length = my_strlen(command);

    if (length == 0) {
        print_error(6);
        return false;
    }

    char **args = split(command, ' ');
    size_t args_len = array_len(args);

    for (size_t i = 0; i < args_len; i++) {
        printf("'%s' ", args[i]);
    }
    
    printf("\n");

    handle_main(args);
    
    return true;
}



// chemin d'accès dynamique ?
int save_blockchain() { 
    char *filename = ".bloackchain.save";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 644);

    if(fd == -1) {
        // throw error
        printf("Error while reading\n");
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


    // printf("[%c%d", sync_state, n_node);

    // char *res = input("]> ", 100);
    // // free en cas de commande incorecte
    

    // printf("echo : '%s'\n", res);
    
    // check_command(res);

    // free(res);


    while(1) {
        char sync_state = 's';
        int n_node = 0;

        printf("[%c%d", sync_state, n_node);

        char *res = input("]> ", 100);
        // free en cas de commande incorecte
        

        printf("echo : '%s'\n", res);
        
        check_command(res);

        free(res);
    }
    return 0;
}