#include <fcntl.h>
#include <stdio.h>
#include <string.h>


typedef struct s_handler {
    const char *command;
    // void (*function_handler)(char **args);
    void (*function_handler)(void);
} t_handler;

// Handler Add
void handle_add_node(char **args) {

}

void handle_add_block(char **args) {

}


// Handler Rm
void handle_rm_node(char **args) {

}

void handle_rm_block(char **args) {

}


void handle_add(char **args) {
    t_handler tabs[] = {
        {"node", handle_add_node},
        {"block", handle_add_block},
        {0, NULL}
    };
}

void handle_rm(char **args) {
    t_handler tabs[] = {
        {"node", handle_rm_node},
        {"block", handle_rm_block},
        {0, NULL}
    };
}

void handle_ls(char **args) {

}

void handle_sync(char **args) {

}

void handle_quit(char **args) {
    
}

void handle_main(char **args) {
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

    }
}