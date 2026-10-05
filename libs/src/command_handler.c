#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "../headers/utils.h"
#include "../headers/command_handler.h"
#include "../headers/node_manager.h"

size_t ARGS_STEP = 0;

// Handler Add
void handle_add_node(t_node **blockchain, char **args) {
    if(!args || !args[ARGS_STEP] || !check_nid(args[ARGS_STEP])) {
        print_error(6);
        return;
    }

    int nid = str_to_int(args[ARGS_STEP]);
    printf("valid nid : %d\n", nid);
    
    add_node(blockchain, nid);
}

void handle_add_block(t_node **blockchain, char **args) {
    if(!args || !args[ARGS_STEP] || !check_bid(args[ARGS_STEP])) {
        print_error(6);
        return;
    }

    printf("%p\n", blockchain);
}

// Handler Rm
void handle_rm_node(t_node **blockchain, char **args) {
    if(!args || !args[ARGS_STEP] || !check_nid(args[ARGS_STEP])) {
        print_error(6);
        return;
    }

    printf("%p\n", blockchain);
}

void handle_rm_block(t_node **blockchain, char **args) {
    if(!args || !args[ARGS_STEP] || !check_bid(args[ARGS_STEP])) {
        print_error(6);
        return;
    }

    printf("%p\n", blockchain);
}


void handle_add(t_node **blockchain, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(6);
        return;
    }

    t_handler tabs[] = {
        {"node", handle_add_node},
        {"block", handle_add_block},
        {"", NULL}
    };

    size_t i = 0;

    while(tabs[i].command[0]) {
        if(my_strcmp(tabs[i].command, args[ARGS_STEP]) == 0) {
            ARGS_STEP++;
            tabs[i].function_handler(blockchain, args);
            break;
        }

        i++;
    }

    if(tabs[i].command[0] == '\0') {
        print_error(6);
        return;
    }
}

void handle_rm(t_node **blockchain, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(6);
        return;
    }

    t_handler tabs[] = {
        {"node", handle_rm_node},
        {"block", handle_rm_block},
        {"", NULL}
    };

    size_t i = 0;

    while(tabs[i].command[0]) {
        if(my_strcmp(tabs[i].command, args[ARGS_STEP]) == 0) {
            ARGS_STEP++;
            tabs[i].function_handler(blockchain, args);
            break;
        }

        i++;
    }

    if(tabs[i].command[0] == '\0') {
        print_error(6);
        return;
    }
}

void handle_ls(t_node **blockchain, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(6);
        return;
    }

    // flags -l ?
    if(args) {
        printf("here");
    }

    printf("%p\n", blockchain);
}

void handle_sync(t_node **blockchain, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(6);
        return;
    }

    // execute sync
    if(args) {
        printf("here");
    }

    printf("%p\n", blockchain);
}

// void handle_quit(t_node **blockchain, char **args) {
//     if(!args || !args[ARGS_STEP]) {
//         print_error(6);
//         return;
//     }


//     // execute quit ?
//     // à faire à part (while(strcmp(args[0], "quit"))) ou renvoyer une valeur différente de toutes les autres fonction
//     if(args) {
//         printf("here");
//     }

//     printf("%p\n", blockchain);
// }

void handle_main(t_node **blockchain, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(6);
        return;
    }

    t_handler tabs[] = {
        {"add", handle_add},
        {"rm", handle_rm},
        {"ls", handle_ls},
        {"sync", handle_sync},
        {"", NULL}
    };

    size_t i = 0;

    while(tabs[i].command[0]) {
        if(my_strcmp(tabs[i].command, args[ARGS_STEP]) == 0) {
            ARGS_STEP++;
            tabs[i].function_handler(blockchain, args);
            break;
        }

        i++;
    }

    if(tabs[i].command[0] == '\0') {
        print_error(6);
        return;
    }
}

void check_command(t_node **blockchain, char *command) {
    size_t length = my_strlen(command);

    if (length == 0) {
        print_error(6);
        return;
    }

    char **args = split(command, ' ');
    if(!args) {
        print_error(6);
        free_split(args);
        return;
    }

    ARGS_STEP = 0;

    handle_main(blockchain, args);

    free_split(args);
}