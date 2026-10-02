#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "../headers/utils.h"


typedef struct s_handler {
    const char *command;
    void (*function_handler)(char **args);
} t_handler;

void handle_add_node(char **args);
void handle_add_block(char **args);
void handle_rm_node(char **args);
void handle_rm_block(char **args);
void handle_add(char **args);
void handle_rm(char **args);
void handle_ls(char **args);
void handle_sync(char **args);
void handle_quit(char **args);
void handle_main(char **args);
void check_command(char *command);

#endif
