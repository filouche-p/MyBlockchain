#include "../headers/utils.h"

void ls(t_node **blockchain_address) {
    t_node *node = *blockchain_address;

    while(node) {
        printf("%d : ", node->nid);

        if(node->first_block) {
            t_block *block = node->first_block;
            
            while(block) {
                printf("%s->", block->bid);
                block = block->next;
            }
        }

        printf("(null)\n");
        node = node->next;
    }

    printf("(null)\n");
}