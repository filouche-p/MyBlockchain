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
#include "./libs/headers/utils.h"
#include "./libs/headers/backup_manager.h"
#include "./libs/headers/command_handler.h"
#include "./libs/headers/command_sup.h"

//malloc, free, printf, write, open, read, close


int main(void) {
    t_node *blockchain = load_blockchain(".blockchain.save");

    char *leak = malloc(100);
    leak[0] = 'A';
    
    while(true) {
        char sync_state = is_sync(&blockchain) ? 's' : '-';
        size_t n_node = blockchain_len(&blockchain);

        printf("[%c%lu", sync_state, n_node);

        char *res = input("]> ", 50);

        if(my_strcmp(res, "quit") == 0) {
            free(res);
            break;
        }

        if(!res) {
            printf("\n");
        }
        //? separate check commande and handle main call
        check_command(&blockchain, res);

        free(res);
    }

    save_blockchain(&blockchain, ".blockchain.save");
    free_blockchain(&blockchain);

    /*
        Is synced = tous les noeuds ont les mêmes blocs
    */

    return 0;
}