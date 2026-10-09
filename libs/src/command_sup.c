#include "../headers/utils.h"
#include "../headers/command_sup.h"

void ls(t_node **blockchain_address, bool show_block) {
    t_node *node = *blockchain_address;

    while(node) {
        printf("%d%s", node->nid, (show_block ? " : " : ""));

        if(node->first_block && show_block) {
            size_t i = 0;
            t_block *block = node->first_block;
            size_t block_size = block_len(blockchain_address, node->nid);
            
            while(i < block_size) {
                if (i < block_size - 1) {
                    printf("%s->", block->bid);
                } else {
                    printf("%s", block->bid);
                }
                block = block->next;
                i++;
            }
        }

        printf("\n");
        node = node->next;
    }
}

size_t blockchain_len(t_node **blockchain_address) {
    size_t i = 0;
    t_node *node = *blockchain_address;

    while(node) {
        node = node->next;
        i++;
    }

    return i;
}

size_t block_len(t_node **blockchain_address, int nid) {
    size_t i = 0;
    t_node *node = *blockchain_address;

    while(node) {
        if (node->nid == nid) {
            t_block *block = node->first_block;

            while (block) {
                i++;
                block = block->next;
            }
        }

        node = node->next;
    }

    return i;
}