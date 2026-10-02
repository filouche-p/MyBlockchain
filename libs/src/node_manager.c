#include "../headers/utils.h"
#include "../headers/node_manager.h"


/*
    NID is ALWAYS supposed to be a correct value (int)
*/
// typedef struct s_block {
//     char *bid;
//     struct s_block *next;
// } t_block;

// typedef struct s_node {
//     int nid;
//     t_block *first_block;
//     struct s_node *next;
// } t_node;

bool add_node(t_node *node, int nid) {
    if (!node) {
        print_error(4);
        return false;
    }
    //assert we are at the last node
    while (node->next) {
        node = node->next;
    }

    // malloc
    node->next = smalloc(sizeof(t_node));
    if(!node->next) {
        print_error(1);
        return false;
    }

    node->next->nid = nid;
    return true;
}

bool rm_node(t_node *node, int nid) {

}
