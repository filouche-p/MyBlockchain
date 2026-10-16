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

void handle_add_node(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
        return;
    } else if (!is_nid_valid(blockchain_adress, args[ARGS_STEP], true)) {
        print_error(NODE_ALREADY_EXISTS);
        return;
        //? sub separated check for more precise error
    }

    int nid = str_to_int(args[ARGS_STEP]);
    add_node(blockchain_adress, nid);
}

void handle_add_block(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
        return;
    }

    char *bid = args[ARGS_STEP];

    if (!args[ARGS_STEP + 1]) {
        print_error(COMMAND_NOT_FOUND);
        return;
    }

    if (args[ARGS_STEP + 1][0] == '*') {
        t_node *node = *blockchain_adress;
        while (node) {
            if (is_bid_valid(node, bid, true)) {
                add_block(blockchain_adress, node->nid, bid);
            }
            node = node->next;
        }
        return;
    }

    if (!can_be_int(args[ARGS_STEP + 1])) {
        print_error(COMMAND_NOT_FOUND);
        return;
    }

    if (!is_bid_valid(*blockchain_adress, bid, true)) {
        print_error(BLOCK_ALREADY_EXISTS);
        return;
    }

    int nid = str_to_int(args[ARGS_STEP + 1]);

    if (!nid_exists(blockchain_adress, nid)) {
        print_error(NODE_DOESNT_EXISTS);
        return;
    }

    add_block(blockchain_adress, nid, bid);
}

void handle_rm_node(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
        return;
    }

    char *supposed_nid = args[ARGS_STEP];

    if (supposed_nid[0] == '*') {
        rm_node(blockchain_adress, 0, true);
        return;
    }

    if (!can_be_int(supposed_nid)) {
        print_error(COMMAND_NOT_FOUND);
        return;
    }

    int nid = str_to_int(supposed_nid);
    rm_node(blockchain_adress, nid, false);
}

void handle_rm_block(t_node **blockchain_adress, char **args) {
    if(!args || !args[ARGS_STEP]) {
        print_error(COMMAND_NOT_FOUND);
        return;
    }

    char *bid = args[ARGS_STEP];

    if (bid[0] == '*' && !args[ARGS_STEP + 1]) {
        t_node *node = *blockchain_adress;
        while (node) {
            rm_block(blockchain_adress, node->nid, "*", true);
            node = node->next;
        }
        return;
    }

    if (!args[ARGS_STEP + 1]) {
        print_error(COMMAND_NOT_FOUND);
        return;
    }

    char *supposed_nid = args[ARGS_STEP + 1];

    if (supposed_nid[0] == '*' && bid[0] != '*') {
        t_node *node = *blockchain_adress;
        while (node) {
            rm_block(blockchain_adress, node->nid, bid, false);
            node = node->next;
        }
        return;
    }

    if (!can_be_int(supposed_nid)) {
        print_error(COMMAND_NOT_FOUND);
        return;
    }

    int nid = str_to_int(supposed_nid);
    rm_block(blockchain_adress, nid, bid, bid[0] == '*');
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
    printf("OK\n");
}

void handle_help(t_node **blockchain_adress, char **args) {
    (void)blockchain_adress;

    static const char *help_add =
        "add -- add a node or a block\n"
        "  add node <nid>        add a node with identifier <nid> (integer)\n"
        "  add block <bid> <nid> add block <bid> to node <nid>\n"
        "  add block <bid> *     add block <bid> to all nodes\n";

    static const char *help_rm =
        "rm -- remove a node or a block\n"
        "  rm node <nid>         remove node <nid> and all its blocks\n"
        "  rm node *             remove all nodes\n"
        "  rm block <bid> <nid>  remove block <bid> from node <nid>\n"
        "  rm block * <nid>      remove all blocks from node <nid>\n"
        "  rm block <bid> *      remove block <bid> from every node\n";

    static const char *help_ls =
        "ls -- list nodes\n"
        "  ls      print all node identifiers\n"
        "  ls -l   print nodes with their blocks\n";

    static const char *help_sync =
        "sync -- synchronize the blockchain\n"
        "  sync    collect all unique blocks from every node and distribute\n"
        "          them to all nodes (result: every node holds the same blocks)\n";

    static const char *help_quit =
        "quit -- exit the program\n"
        "  quit    save the blockchain and terminate the program\n";

    static const char *help_help =
        "help -- display help\n"
        "  help           list all available commands\n"
        "  help <cmd>     show detailed help for <cmd>\n"
        "                 commands: add, rm, ls, sync, quit, help\n";

    static const char *help_all =
        "available commands:\n"
        "  add    add a node or a block\n"
        "  rm     remove a node or a block\n"
        "  ls     list nodes (use -l to also show blocks)\n"
        "  sync   synchronize all nodes\n"
        "  quit   save and exit\n"
        "  help   show this help  |  help <cmd> for details\n";

    if (!args || !args[ARGS_STEP]) {
        printf("%s", help_all);
        return;
    }

    typedef struct { const char *name; const char *text; } t_help_entry;
    t_help_entry entries[] = {
        {"add",  help_add},
        {"rm",   help_rm},
        {"ls",   help_ls},
        {"sync", help_sync},
        {"quit", help_quit},
        {"help", help_help},
        {NULL,   NULL}
    };

    size_t i = 0;
    while (entries[i].name) {
        if (my_strcmp(entries[i].name, args[ARGS_STEP]) == 0) {
            printf("%s", entries[i].text);
            return;
        }
        i++;
    }

    print_error(COMMAND_NOT_FOUND);
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
        {"help", handle_help},
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

    ARGS_STEP = 0;
    handle_main(blockchain_adress, args);
    free_array(args);
}