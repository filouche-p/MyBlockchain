#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "./libs/headers/utils.h"
#include "./libs/headers/backup_manager.h"
#include "./libs/headers/command_handler.h"
#include "./libs/headers/command_sup.h"
#include "./libs/headers/input.h"

int main(void) {
    t_node *blockchain = load_blockchain(".blockchain.save");
    printf("type \"help\" or \"help <cmd>\" to view command\n");
    if(!blockchain) {
        printf("No Backup Found: Starting New Blockchain\n");
    } else {
        printf("Restoring From Backup\n");
    }

    t_history history = {NULL, 0, 0};
    char *completions[] = {
        "add node", "add block", "rm node", "rm block",
        "ls", "ls -l", "sync", "quit", "help",
        "help add", "help rm", "help ls", "help sync", "help quit", "help help",
        NULL
    };

    while(true) {
        char sync_state = is_sync(&blockchain) ? 's' : '-';
        size_t n_node = blockchain_len(&blockchain);
        char prompt[64];

        snprintf(prompt, sizeof(prompt), "[%c%lu]> ", sync_state, n_node);

        char *res = custom_input(prompt, 50, &history, completions);
        if (!res) {
            printf("\n");
            break;
        }

        if (my_strcmp(res, "quit") == 0) {
            free(res);
            break;
        }

        if (*res) {
            history_add(&history, res);
        }

        check_command(&blockchain, res);
        free(res);
    }

    history_free(&history);
    save_blockchain(&blockchain, ".blockchain.save");
    free_blockchain(&blockchain);

    printf("Backing up blockchain...\n");

    return 0;
}
