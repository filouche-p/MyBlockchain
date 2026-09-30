#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "../headers/utils.h"

size_t ARGS_STEP = 0;

typedef struct s_handler {
    const char *command;
    void (*function_handler)(char **args);
} t_handler;

// Handler Add
void handle_add_node(char **args) {
    // execute add node args[ARG_STEP] = nid ?
    printf("Check for : %s\n", args[ARGS_STEP]);
    if(!check_nid(args[ARGS_STEP])) {
        print_error(6);
    }
}

void handle_add_block(char **args) {
    // execute add block args[ARG_STEP] = bid ?
    if(!check_bid(args[ARGS_STEP])) {
        print_error(6);
    }
}

// Handler Rm
void handle_rm_node(char **args) {
    // execute rm node args[ARG_STEP] = nid ?
    if(!check_nid(args[ARGS_STEP])) {
        print_error(6);
    }
}

void handle_rm_block(char **args) {
    // execute rm node args[ARG_STEP] = bid ?
    if(!check_bid(args[ARGS_STEP])) {
        print_error(6);
    }
}


void handle_add(char **args) {
    t_handler tabs[] = {
        {"node", handle_add_node},
        {"block", handle_add_block},
        {"\0", NULL}
    };

    size_t i = 0;

    while(tabs[i].command[0]) {
        if(_strcmp(tabs[i].command, args[ARGS_STEP]) == 0) {
            ARGS_STEP++;
            tabs[i].function_handler(args);
            break;
        }
    }

    if(!tabs[i].command) {
        print_error(6);
    }
}

void handle_rm(char **args) {
    t_handler tabs[] = {
        {"node", handle_rm_node},
        {"block", handle_rm_block},
        {"\0", NULL}
    };

    size_t i = 0;

    while(tabs[i].command[0]) {
        if(_strcmp(tabs[i].command, args[ARGS_STEP])) {
            ARGS_STEP++;
            tabs[i].function_handler(args);
            break;
        }
    }

    if(!tabs[i].command) {
        print_error(6);
    }
}

void handle_ls(char **args) {
    // flags -l ?
}

void handle_sync(char **args) {
    // execute sync
}

void handle_quit(char **args) {
    // execute quit ?
    // à faire à part (while(strcmp(args[0], "quit"))) ou renvoyer une valeur différente de toutes les autres fonction
}

void handle_main(char **args) {
    t_handler tabs[] = {
        {"add", handle_add},
        {"rm", handle_rm},
        {"ls", handle_ls},
        {"sync", handle_sync},
        {"quit", handle_quit},
        {"\0", NULL}
    };

    size_t i = 0;

    while(tabs[i].command[0]) {
        if(_strcmp(tabs[i].command, args[ARGS_STEP]) == 0) {
            ARGS_STEP++;
            tabs[i].function_handler(args);
            break;
        }
    }

    if(!tabs[i].command) {
        print_error(6);
    }
}