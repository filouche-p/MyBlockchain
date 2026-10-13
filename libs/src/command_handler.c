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
#include "../headers/command_sup.h"

size_t ARGS_STEP = 0;

// Handler Add
void handle_add_node(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
        return;
    } else if (!check_nid(blockchain_adress, args[ARGS_STEP])) {
        print_error(NODE_ALREADY_EXISTS);
        return;
        //! sub separated check for more precise error
    }

    printf("deprecated, print command not found when nid already exists\n");

    int nid = str_to_int(args[ARGS_STEP]);
    add_node(blockchain_adress, nid);
}

void handle_add_block(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
        return;
    } else if (!check_bid(args[ARGS_STEP])) {
        print_error(BLOCK_ALREADY_EXISTS);
        return;
    } else if (!args[ARGS_STEP + 1] || !can_be_int(args[ARGS_STEP + 1])) {
        // we don't use check_nid cause it check for unicity
        print_error(NODE_ALREADY_EXISTS);
        return;
    }

    printf("deprecated work even if nid doest exist (no error printed)\n");

    int nid = str_to_int(args[ARGS_STEP + 1]);
    char *bid = args[ARGS_STEP];

    add_block(blockchain_adress, nid, bid);
}

// Handler Rm
void handle_rm_node(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
        return;
    }

    char *supposed_nid = args[ARGS_STEP];
    if (supposed_nid && my_strlen(supposed_nid) >= 1 && supposed_nid[0] == '*') {
        rm_node(blockchain_adress, 0, true); // we doesnt care of the nid if delete_all is set to *
    } else {
        int nid = str_to_int(args[ARGS_STEP]);
        rm_node(blockchain_adress, nid, false);
    }
}

void handle_rm_block(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
        return;
    } else if (!check_bid(args[ARGS_STEP])) {
        print_error(BLOCK_ALREADY_EXISTS);
        return;
    } else if (!args[ARGS_STEP + 1] || !can_be_int(args[ARGS_STEP + 1])) {
        // we don't use check_nid cause it check for unicity
        print_error(NODE_ALREADY_EXISTS);
        return;
    }

    int nid = str_to_int(args[ARGS_STEP + 1]);
    char *bid = args[ARGS_STEP];

    rm_block(blockchain_adress, nid, bid);
}


void handle_add(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
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
        print_error(COMMAND_NOT_FOUND);
        return;
    }
}

void handle_rm(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
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
        print_error(COMMAND_NOT_FOUND);
        return;
    }
}

void handle_ls(t_node **blockchain_adress, char **args) {
    (void)args;
    ls(blockchain_adress, args[ARGS_STEP] && my_strcmp(args[ARGS_STEP], "-l") == 0);
}

void handle_sync(t_node **blockchain_adress, char **args) {
    (void)args;
    sync_blockchain(blockchain_adress);
}

void handle_main(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
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
        print_error(COMMAND_NOT_FOUND);
        return;
    }
}

void check_command(t_node **blockchain_adress, char *command) {
    size_t length = my_strlen(command);

    if (length == 0) {
        print_error(COMMAND_NOT_FOUND);
        return;
    }

    char **args = split(command, ' ');
    if(!args) {
        print_error(COMMAND_NOT_FOUND);
        free_array(args);
        return;
    }

    // size_t args_len = array_len(args);

    // for(size_t i = 0; i < args_len; i++) {
    //     printf("%s\n", args[i]);
    // }

    ARGS_STEP = 0;

    handle_main(blockchain_adress, args);

    free_array(args);
}