#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct s_block {
    char *bid;
    struct s_block *next;
} t_block;

typedef struct s_node {
    int nid;
    t_block *first_block;
    struct s_node *next;
} t_node;

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


void show_list(t_node **blockchain_address) {
    t_node *node = *blockchain_address;

    // printf("debug sl+ : %p\n", node);

    // printf("debug_sl : %d | %p\n", node->nid, node->next);
    while (node) {
        printf("v : %d\n", node->nid);
        node = node->next;
    }

    return;
}


int main(void) {
    t_node *og = malloc(sizeof(struct s_node));
    t_node *first = malloc(sizeof(struct s_node));
    
    og->nid = 0;
    og->next = first;

    first->nid = 1;
    first->next = NULL;

    show_list(&og);

    printf("%d - a : %p, n : %p | %d - a: %p n: %p\n", og->nid, og, og->next, first->nid, first, first->next);

    t_node *res = add_node(&og, 2);

    printf("%d - a : %p, n : %p | %d - a: %p n: %p\n", og->nid, og, og->next, first->nid, first, first->next);


    show_list(&og);

    free(og);
    free(first);
    free(res);

    return 0;
}