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
                return true; // it fail but not bc of classic error
            }

            new_block->bid = bid;
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