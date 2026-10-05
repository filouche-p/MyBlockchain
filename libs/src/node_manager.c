#include "../headers/utils.h"
#include "../headers/node_manager.h"


/*
    NID is ALWAYS supposed to be a correct value (int)
*/

t_node *add_node(t_node **blockchain_address, int nid) {
    t_node *new_node = malloc(sizeof(t_node));
    if (!new_node) {
        printf("error : 1\n"); // free ?
        return NULL;
    }

    new_node->nid = nid;
    new_node->next = NULL;
    
    // empty list
    if (*blockchain_address == NULL) { 
        *blockchain_address = new_node;
    }

    t_node *node = *blockchain_address;
    
    while (node->next != NULL) {
        node = node->next;
    }
    
    node->next = new_node;

    return new_node;
}

// bool add_node(t_node *node, int nid) {
//     if (!node) {
//         print_error(4);
//         return false;
//     }
//     //assert we are at the last node
//     while (node->next) {
//         node = node->next;
//     }

//     // malloc
//     node->next = smalloc(sizeof(t_node));
//     if(!node->next) {
//         print_error(1);
//         return false;
//     }

//     node->next->nid = nid;
//     return true;
// }

bool rm_node(t_node *node, int nid) {

}
