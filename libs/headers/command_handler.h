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
    void (*function_handler)(t_node **blockchain, char **args);
} t_handler;

void handle_add_node(t_node **blockchain, char **args);
void handle_add_block(t_node **blockchain, char **args);
void handle_rm_node(t_node **blockchain, char **args);
void handle_rm_block(t_node **blockchain, char **args);
void handle_add(t_node **blockchain, char **args);
void handle_rm(t_node **blockchain, char **args);
void handle_ls(t_node **blockchain, char **args);
void handle_sync(t_node **blockchain, char **args);
void handle_main(t_node **blockchain, char **args);
void check_command(t_node **blockchain, char *command);

#endif
