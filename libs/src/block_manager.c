#include "../headers/block_manager.h"
#include "../headers/utils.h"

/*
    BID is ALWAYS supposed true in these functions
*/

bool add_block(t_node **blockchain_adress, int nid, const char *bid) {
    if (blockchain_adress == NULL || *blockchain_adress == NULL) {
        return false;
    }

    t_node *node = *blockchain_adress;

    while(node != NULL) {
        if(node->nid == nid) {
            t_block *new_block = smalloc(sizeof(t_block));
            if (!new_block) {
                print_error(1);
                return true; // it fail but not bc of classic error so we return true to avoid double printing
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

            return true;
        }

        node = node->next;
    }

    return false;
}

bool rm_block(t_node **blockchain_address, int nid, const char *bid) {
    t_node *node = *blockchain_address;

    while (node) {
        if (node->nid == nid) {
            t_block *block = node->first_block;
            printf("rm b: %p\n", block);
            while (block) {
                // if(block->bid == bid)
                if(block->next->bid == bid) {
                    if (block->next->next) {
                        block->next = block->next->next;
                    } else {
                        block->next = NULL;
                    }

                    free(block->next);
                    break;
                }

                if(!block->next) {
                    printf("rm b: %p->%p\n", block, block->next);
                    print_error(5); // block doesn't exist
                    return false;
                }

                block = block->next;
            }
        }
        if (!node->next) {
            print_error(4); // nid doesn't exist 
            return false;
        }

        node = node->next;
    }

    return false;
}