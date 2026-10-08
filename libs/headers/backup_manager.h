#ifndef BACKUP_MANAGER_H
#define BACKUP_MANAGER_H

t_node *load_blockchain(const char *file_path);
void save_blockchain(t_node **blockchain_address, char *file_name);

#endif