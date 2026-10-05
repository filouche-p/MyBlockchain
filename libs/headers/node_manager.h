#ifndef NODE_MANAGER_H
#define NODE_MANAGER_H

#include "./utils.h"

void add_node(t_node **blockchain_address, int nid);
bool rm_node(t_node **blockchain_address, int nid);
void ls_node(t_node **blockchain_address);

#endif
