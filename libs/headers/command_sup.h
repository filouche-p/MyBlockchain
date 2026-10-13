#ifndef COMMAND_SUP_H
#define COMMAND_SUP_H

#include "./utils.h"

void ls(t_node **blockchain_address, bool show_block);
size_t blockchain_len(t_node **blockchain_address);
size_t get_node_block_len(t_node *node);
char **get_blocks(t_node *node);
bool is_sync(t_node **blockchain_address);
void sync_blockchain(t_node **blockchain_address);

#endif