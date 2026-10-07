#include "../headers/block_manager.h"
#include "../headers/utils.h"

/*
    BID is ALWAYS supposed true in these functions
*/

void add_block(t_node **blockchain_adress, int nid, const char *bid) {
    if (blockchain_adress == NULL || *blockchain_adress == NULL) {
        return;
    }

    t_node *node = *blockchain_adress;

    while(node != NULL) {
        if(node->nid == nid) {
            t_block *new_block = smalloc(sizeof(t_block));
            if (!new_block) {
                print_error(NO_MORE_RESSOURCES);
                return; // it fail but not bc of classic error so we return true to avoid double printing
            }

            new_block->bid = my_strdup(bid);
            new_block->next = NULL;

            if (node->first_block == NULL) {
                node->first_block = new_block;
            } else {
                t_block *block = node->first_block;
                while (block->next != NULL) {
                    block = block->next;
                }

                block->next = new_block;
            }

            return;
        }

        node = node->next;
    }

    return;
}

void rm_block(t_node **blockchain_address, int nid, const char *bid) {
    if (!blockchain_address || !*blockchain_address) {
        print_error(NODE_DOESNT_EXISTS);
        return;
    }

    t_node *node = *blockchain_address;

    while (node) {
        if (node->nid == nid) {
            t_block *block = node->first_block;
            t_block *prev = NULL;

            while (block) {
                if (my_strcmp(block->bid, bid) == 0) {
                    if (prev == NULL) {
                        node->first_block = block->next;
                    } else {
                        prev->next = block->next;
                    }

                    free(block->bid);
                    free(block);
                    return;
                }

                prev = block;
                block = block->next;
            }

            print_error(BLOCK_DOESNT_EXISTS);
            return;
        }

        node = node->next;
    }

    print_error(NODE_DOESNT_EXISTS);
    return;
}