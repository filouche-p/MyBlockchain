#ifndef BLOCK_MANAGER_H
#define BLOCK_MANAGER_H

#include "./utils.h"

bool add_block(t_node **blockchain_adress, int nid, const char *bid);
bool rm_block(t_node **blockchain_address, int nid, const char *bid);

#endif