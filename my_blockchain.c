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
    //!sync
    t_node *blockchain = load_blockchain(".blockchain.save");
    if (blockchain) {
        printf("successfully loaded blockchain at %p\n", blockchain);
        printf("first node : %d\n", blockchain->nid);
        printf("%lu\n", blockchain_len(&blockchain));
    }


    while(true) {
        char sync_state = 's';
        int n_node = 0;

        printf("[%c%d", sync_state, n_node);

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

    return 0;
}