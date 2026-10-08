#ifndef COMMAND_SUP_H
#define COMMAND_SUP_H

#include "./utils.h"

void ls(t_node **blockchain_address, bool show_block);
size_t blockchain_len(t_node **blockchain_address);
size_t block_len(t_node **blockchain_address, int nid);

#endif