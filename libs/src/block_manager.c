#include "../headers/block_manager.h"
#include "../headers/utils.h"

/*
    BID is ALWAYS supposed true in these functions
*/

void add_block(t_node **blockchain_adress, int nid, const char *bid) {
    if (blockchain_adress == NULL || *blockchain_adress == NULL) {
        return;
    }
    if(!bid || bid[0] == '*') {
        print_error(BLOCK_DOESNT_EXISTS);
        return;
    }

    t_node *node = *blockchain_adress;

    while(node != NULL) {
        if(node->nid == nid) {
            t_block *new_block = smalloc(sizeof(t_block));
            if (!new_block) {
                print_error(NO_MORE_RESSOURCES);
                return;
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

void rm_block(t_node **blockchain_address, int nid, const char *bid, bool delete_all) {
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
                if (delete_all || (my_strcmp(block->bid, bid) == 0)) {
                    t_block *to_free = block;

                    if (prev == NULL) {
                        node->first_block = block->next;
                    } else {
                        prev->next = block->next;
                    }

                    block = block->next; 
                    free(to_free->bid);
                    free(to_free);
                    
                    if (!delete_all) {
                        return;
                    }
                } else {
                    prev = block;
                    block = block->next;
                }
            }

            return;
        }
        node = node->next;
    }

    print_error(NODE_DOESNT_EXISTS);
}