#include "../headers/utils.h"
#include "../headers/command_sup.h"
#include "../headers/block_manager.h"

void ls(t_node **blockchain_address, bool show_block) {
    t_node *node = *blockchain_address;

    while(node) {
        printf("%d%s", node->nid, (show_block ? " : " : ""));

        if(node->first_block && show_block) {
            size_t i = 0;
            t_block *block = node->first_block;
            size_t block_size = get_node_block_len(node);
            
            while(i < block_size) {
                printf("%s%s", block->bid, (i < block_size - 1) ? "-" : "");
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

size_t get_node_block_len(t_node *node) {
    if (!node) return 0;
    
    size_t len = 0;
    t_block *block = node->first_block;
    
    while (block) {
        len++;
        block = block->next;
    }
    
    return len;
}

char **get_blocks(t_node *node) {
    if (!node) {
        return NULL;
    }

    size_t node_length = 0;
    t_block *temp = node->first_block;
    while (temp) {
        node_length++;
        temp = temp->next;
    }

    char **res = smalloc((node_length + 1) * sizeof(char *));
    if (!res) {
        return NULL;
    }

    size_t index = 0;
    t_block *block = node->first_block;
    while (block) {
        res[index] = block->bid;
        index++;
        block = block->next;
    }

    res[index] = NULL;
    return res;
}

bool is_sync(t_node **blockchain_address) {
    if (!blockchain_address || !*blockchain_address || !(*blockchain_address)->next) {
        return true;
    }

    t_node *base_node = *blockchain_address;
    t_node *current_node = base_node->next;

    char **base_blocks = get_blocks(base_node);
    if (!base_blocks) return false;
    
    size_t base_len = array_len(base_blocks);

    while (current_node) {
        char **current_blocks = get_blocks(current_node);
        if (!current_blocks) {
            free(base_blocks);
            return false;
        }

        if (array_len(current_blocks) != base_len) {
            free(base_blocks);
            free(current_blocks);
            return false;
        }

        char **diff = array_diff(base_blocks, current_blocks); 
        
        if (!diff || diff[0] != NULL) {
            free(base_blocks);
            free(current_blocks);
            if (diff) {
                free_array(diff);
            }
            return false;
        }

        free(current_blocks);
        free_array(diff);
        
        current_node = current_node->next;
    }

    free(base_blocks);
    
    return true;
}

void sync_blockchain(t_node **blockchain_address) {
    if (!blockchain_address || !*blockchain_address || !(*blockchain_address)->next) {
        return;
    }

    t_node *base_node = *blockchain_address;
    t_node *current_node;

    current_node = base_node->next;
    while (current_node) {
        char **base_blocks = get_blocks(base_node);
        char **current_blocks = get_blocks(current_node);

        if (base_blocks && current_blocks) {
            char **diff = array_diff(current_blocks, base_blocks);
            
            if (diff) {
                size_t k = 0;
                while (diff[k]) {
                    add_block(blockchain_address, base_node->nid, diff[k]);
                    free(diff[k]);
                    k++;
                }
                free(diff);
            }
        }
        
        if (base_blocks) free(base_blocks);
        if (current_blocks) free(current_blocks);

        current_node = current_node->next;
    }

    char **master_blocks = get_blocks(base_node);
    if (!master_blocks) return;

    current_node = base_node->next;
    while (current_node) {
        char **current_blocks = get_blocks(current_node);
        
        if (current_blocks) {
            char **diff = array_diff(master_blocks, current_blocks);
            
            if (diff) {
                size_t k = 0;
                while (diff[k]) {
                    add_block(blockchain_address, current_node->nid, diff[k]);
                    free(diff[k]);
                    k++;
                }
                free(diff);
            }
            free(current_blocks);
        }
        
        current_node = current_node->next;
    }

    free(master_blocks);
}