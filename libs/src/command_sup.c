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

    size_t master_capacity = 64;
    size_t master_len = 0;
    char **master = smalloc(master_capacity * sizeof(char *));
    if (!master) return;

    t_node *node = *blockchain_address;
    while (node) {
        t_block *block = node->first_block;
        while (block) {
            bool already_in_master = false;
            for (size_t k = 0; k < master_len; k++) {
                if (my_strcmp(master[k], block->bid) == 0) {
                    already_in_master = true;
                    break;
                }
            }
            if (!already_in_master) {
                if (master_len + 1 >= master_capacity) {
                    master_capacity *= 2;
                    char **tmp = smalloc(master_capacity * sizeof(char *));
                    if (!tmp) {
                        for (size_t k = 0; k < master_len; k++) free(master[k]);
                        free(master);
                        return;
                    }
                    for (size_t k = 0; k < master_len; k++) tmp[k] = master[k];
                    free(master);
                    master = tmp;
                }
                master[master_len] = my_strdup(block->bid);
                if (!master[master_len]) {
                    for (size_t k = 0; k < master_len; k++) free(master[k]);
                    free(master);
                    return;
                }
                master_len++;
            }
            block = block->next;
        }
        node = node->next;
    }
    master[master_len] = NULL;

    node = *blockchain_address;
    while (node) {
        for (size_t k = 0; k < master_len; k++) {
            if (is_bid_valid(node, master[k], true)) {
                add_block(blockchain_address, node->nid, master[k]);
            }
        }
        node = node->next;
    }

    for (size_t k = 0; k < master_len; k++) free(master[k]);
    free(master);
}