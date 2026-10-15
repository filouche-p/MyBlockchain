#include "../headers/utils.h"
#include "../headers/node_manager.h"
#include "../headers/block_manager.h"

/*
    NID is ALWAYS supposed to be a correct value (int)
*/

void add_node(t_node **blockchain_address, int nid) {
    t_node *new_node = smalloc(sizeof(t_node));
    t_node *node = *blockchain_address;

    if (!new_node) {
        print_error(NO_MORE_RESSOURCES);
        return;
    }

    new_node->nid = nid;
    new_node->next = NULL;
    new_node->first_block = NULL;
    
    if (!(*blockchain_address)) {
        *blockchain_address = new_node;
        return;
    }
    
    while (node->next != NULL) {
        node = node->next;
    }
    
    node->next = new_node;
    return;
}

void rm_node(t_node **blockchain_address, int nid, bool delete_all) {
    if (!blockchain_address || !*blockchain_address) {
        return;
    }

    t_node *current = *blockchain_address;
    t_node *prev = NULL;

    while (current) {
        if(delete_all || current->nid == nid) {
            if(current->first_block) {
                rm_block(blockchain_address, current->nid, "*", true);
            }

            t_node *to_free = current;
            
            if(prev == NULL) {
                *blockchain_address = current->next;
            } else {
                prev->next = current->next;
            }

            current = current->next;
            free(to_free);

            if (!delete_all) {
                return;
            }
        } else {
            prev = current;
            current = current->next;
        }
    }
}