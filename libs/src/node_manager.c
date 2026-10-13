#include "../headers/utils.h"
#include "../headers/node_manager.h"
#include "../headers/block_manager.h"

/*
    NID is ALWAYS supposed to be a correct value (int)
*/

void add_node(t_node **blockchain_address, int nid) {
    t_node *new_node = smalloc(sizeof(t_node));
    t_node *node = *blockchain_address; // *blockchain_address is the head

    if (!new_node) {
        print_error(NO_MORE_RESSOURCES);
        return;
    }

    new_node->nid = nid;
    new_node->next = NULL;
    new_node->first_block = NULL;
    
    // empty list
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
    if (!blockchain_address || !(*blockchain_address)) {
        return;
    }

    t_node *head = *blockchain_address;
    t_node *temp_to_free = NULL;

    if (delete_all || (head->nid == nid)) {
        *blockchain_address = head->next;
        free(head);
        if (!delete_all) {
            return;
        }
    }

    t_node *node = delete_all ? (*blockchain_address)->next : head;

    while (node->next != NULL) {
        if (delete_all || (node->next->nid == nid)) {
            if (node->first_block) {
                rm_block(blockchain_address, nid, "*");
            }
            temp_to_free = node->next;
            node->next = node->next->next;
            free(temp_to_free);
        } else {
            node = node->next;
        }
    }

    return;
}
