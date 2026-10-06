#include "../headers/utils.h"
#include "../headers/node_manager.h"

/*
    NID is ALWAYS supposed to be a correct value (int)
*/

void add_node(t_node **blockchain_address, int nid) {
    t_node *new_node = malloc(sizeof(t_node));
    t_node *node = *blockchain_address; // *blockchain_address is the head

    if (!new_node) {
        print_error(1); //! free ?
        return;
    }

    new_node->nid = nid;
    new_node->next = NULL;
    
    // empty list
    if (!(*blockchain_address)) {
        *blockchain_address = new_node;
        return;
    }
    
    while (node->next != NULL) {
        node = node->next;
    }
    
    node->next = new_node;
}

void ls_node(t_node **blockchain_address) {
    // print();
    t_node *node = *blockchain_address;

    while (node) {
        printf("%d->", node->nid);
        node = node->next;
    }

    printf("(null)\n");
}

bool rm_node(t_node **blockchain_address, int nid) {
    if (!blockchain_address || !(*blockchain_address)) {
        printf("rm : empty blockchain adress\n");
        return false;
    }

    t_node *head = *blockchain_address;
    t_node *temp_to_free = NULL;

    if (head->nid == nid) {
        *blockchain_address = head->next;
        free(head);
        return true;
    }

    t_node *node = head;

    while (node->next != NULL) {
        if (node->next->nid == nid) {
            temp_to_free = node->next;
            node->next = node->next->next;
            free(temp_to_free);
            return true; // We can exit the function cause nid is unique to every node
        } else {
            node = node->next;
        }
    }

    return false;
}
