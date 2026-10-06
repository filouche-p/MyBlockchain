#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "../headers/utils.h"
#include "../headers/command_handler.h"
#include "../headers/node_manager.h"
#include "../headers/block_manager.h"

size_t ARGS_STEP = 0;

// Handler Add
void handle_add_node(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(6);
        return;
    } else if (!check_nid(blockchain_adress, args[ARGS_STEP])) {
        print_error(2);
        return;
        //! sub separated check for more precise error
    }

    int nid = str_to_int(args[ARGS_STEP]);
    
    bool res = add_node(blockchain_adress, nid);
    if(!res) {
        //? impossible ?
        print_error(2);
        return;
    }
}

void handle_add_block(t_node **blockchain_adress, char **args) {
    printf("currently adding block\n");
    if(!args || !args[ARGS_STEP]) {
        print_error(6);
        return;
    } else if (!check_bid(args[ARGS_STEP])) {
        print_error(3);
        return;
    } else if (!args[ARGS_STEP + 1] || !can_be_int(args[ARGS_STEP + 1])) {
        // we don't use check_nid cause it check for unicity
        print_error(2);
        return;
    }

    int nid = str_to_int(args[ARGS_STEP + 1]);
    char *bid = args[ARGS_STEP];

    bool res = add_block(blockchain_adress, nid, bid);
    if (!res) {
        // we don't have enought info to print correct error
        print_error(1);
        return;
    }
}

// Handler Rm
void handle_rm_node(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(6);
        return;
    }

    int nid = str_to_int(args[ARGS_STEP]);
    bool success = rm_node(blockchain_adress, nid);
    if(!success) {
        print_error(4);
    }
}

void handle_rm_block(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP] || !check_bid(args[ARGS_STEP])) {
        print_error(6);
        return;
    }

    printf("%p\n", blockchain_adress);
}


void handle_add(t_node **blockchain_adress, char **args) {
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
            tabs[i].function_handler(blockchain_adress, args);
            break;
        }

        i++;
    }

    if(tabs[i].command[0] == '\0') {
        print_error(6);
        return;
    }
}

void handle_rm(t_node **blockchain_adress, char **args) {
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
            tabs[i].function_handler(blockchain_adress, args);
            break;
        }

        i++;
    }

    if(tabs[i].command[0] == '\0') {
        print_error(6);
        return;
    }
}

void handle_ls(t_node **blockchain_adress, char **args) {
    // if(!args || !args[ARGS_STEP]) {
    //     printf("no args ls detect\n");
    //     print_error(6);
    //     return;
    // }
    // printf("%s\n", args[0]);
    // flags -l ?

    // printf("%p\n", blockchain);

    (void)args;
    ls_node(blockchain_adress);
}

void handle_sync(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(6);
        return;
    }

    // execute sync
    if(args) {
        printf("here");
    }

    printf("%p\n", blockchain_adress);
}

void handle_main(t_node **blockchain_adress, char **args) {
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
            tabs[i].function_handler(blockchain_adress, args);
            break;
        }

        i++;
    }

    if(tabs[i].command[0] == '\0') {
        print_error(6);
        return;
    }
}

void check_command(t_node **blockchain_adress, char *command) {
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

    // size_t args_len = array_len(args);

    // for(size_t i = 0; i < args_len; i++) {
    //     printf("%s\n", args[i]);
    // }

    ARGS_STEP = 0;

    handle_main(blockchain_adress, args);

    free_split(args);
}