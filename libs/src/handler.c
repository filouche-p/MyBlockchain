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
#include "../headers/utils.h"

size_t ARGS_STEP = 0;

typedef struct s_handler {
    const char *command;
    void (*function_handler)(char **args);
} t_handler;

// Handler Add
void handle_add_node(char **args) {
    printf("add_node : checking for : '%s'\n", args[ARGS_STEP]);

    // execute add node args[ARG_STEP] = nid ?
    if(!args[ARGS_STEP] || !check_nid(args[ARGS_STEP])) {
        ARGS_STEP = 0;
        print_error(6);
        return;
    }

    printf("valid nid : %s\n", args[ARGS_STEP]);
}

void handle_add_block(char **args) {
    printf("add_block : checking for : '%s'\n", args[ARGS_STEP]);

    // execute add block args[ARG_STEP] = bid ?
    if(!check_bid(args[ARGS_STEP])) {
        ARGS_STEP = 0;
        print_error(6);
        return;
    }
}

// Handler Rm
void handle_rm_node(char **args) {
    printf("rm_node : checking for : '%s'\n", args[ARGS_STEP]);

    // execute rm node args[ARG_STEP] = nid ?
    if(!check_nid(args[ARGS_STEP])) {
        ARGS_STEP = 0;
        print_error(6);
        return;
    }
}

void handle_rm_block(char **args) {
    printf("rm_block : checking for : '%s'\n", args[ARGS_STEP]);

    // execute rm node args[ARG_STEP] = bid ?
    if(!check_bid(args[ARGS_STEP])) {
        ARGS_STEP = 0;
        print_error(6);
        return;
    }
}


void handle_add(char **args) {
    printf("add : checking for : '%s'\n", args[ARGS_STEP]);

    t_handler tabs[] = {
        {"node", handle_add_node},
        {"block", handle_add_block},
        {"", NULL}
    };

    size_t i = 0;

    while(tabs[i].command[0]) {
        if(my_strcmp(tabs[i].command, args[ARGS_STEP]) == 0) {
            ARGS_STEP++;
            tabs[i].function_handler(args);
            break;
        }

        i++;
    }

    if(!tabs[i].command) {
        ARGS_STEP = 0;
        print_error(6);
        return;
    }
}

void handle_rm(char **args) {
    printf("rm : checking for : '%s'\n", args[ARGS_STEP]);

    t_handler tabs[] = {
        {"node", handle_rm_node},
        {"block", handle_rm_block},
        {"", NULL}
    };

    size_t i = 0;

    while(tabs[i].command[0]) {
        if(my_strcmp(tabs[i].command, args[ARGS_STEP]) == 0) {
            ARGS_STEP++;
            tabs[i].function_handler(args);
            break;
        }

        i++;
    }

    if(!tabs[i].command) {
        ARGS_STEP = 0;
        print_error(6);
        return;
    }
}

void handle_ls(char **args) {
    printf("ls : checking for : '%s'\n", args[ARGS_STEP]);

    // flags -l ?
    if(args) {
        printf("here");
    }
}

void handle_sync(char **args) {
    printf("sync : checking for : '%s'\n", args[ARGS_STEP]);

    // execute sync
    if(args) {
        printf("here");
    }
}

void handle_quit(char **args) {
    printf("quit : checking for : '%s'\n", args[ARGS_STEP]);

    // execute quit ?
    // à faire à part (while(strcmp(args[0], "quit"))) ou renvoyer une valeur différente de toutes les autres fonction
    if(args) {
        printf("here");
    }
}

void handle_main(char **args) {
    printf("main : checking for : '%s'\n", args[ARGS_STEP]);
    t_handler tabs[] = {
        {"add", handle_add},
        {"rm", handle_rm},
        {"ls", handle_ls},
        {"sync", handle_sync},
        {"quit", handle_quit},
        {"", NULL}
    };

    size_t i = 0;

    while(tabs[i].command[0]) {
        if(my_strcmp(tabs[i].command, args[ARGS_STEP]) == 0) {
            ARGS_STEP++;
            tabs[i].function_handler(args);
            break;
        }

        i++;
    }

    if(!tabs[i].command) {
        ARGS_STEP = 0;
        print_error(6);
        return;
    }
}