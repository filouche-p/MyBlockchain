#include "../headers/utils.h"
#include "../headers/node_manager.h"
#include "../headers/block_manager.h"


/*
    ASSUMING that the data read in backup file are 100% correct
*/
//* load blockchain from FILE to RAM
t_node *load_blockchain(const char *file_path) {
    // internal call we don't check for file path
    size_t block_size_read = 50;
    t_node *blockchain = NULL;

    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        // no backup or very bad error (assuming bad error)
        return NULL;
    }

    // we read line by line, each line is a node, everything after ':' is a block each block separated by a space
    char *buff = smalloc(2048);
    if (!buff) {
        print_error(NO_MORE_RESSOURCES);
        return NULL;
    }

    char **current_node;
    ssize_t bytes_read;

    size_t y = 0;

    // we read until read return 0 (eof)
    // read one by one until we found \n save to variable one for the start one for the end read start from the end -> you got one line = one node, split on space ("split on ->"?) tou got block
    while ((bytes_read = read(fd, buff, block_size_read)) > 0) {
        printf("new read\n");
        current_node = split(split(buff, '\n')[y], ' ');

        int nid = str_to_int(current_node[0]);

        add_node(&blockchain, nid);

        size_t i = 1;
        while (current_node[i]) {
            printf("new block\n");
            if (strcmp(current_node[i], ":") == 0) {
                i++;
                continue;
            }

            char *bid = my_strdup(current_node[i]);
            add_block(&blockchain, nid, bid);
            printf("block added : %d : %s\n", nid, bid);
            i++;
        }

        y++;
    }

    if (bytes_read == -1) {
        // error while reading
        print_error(NO_MORE_RESSOURCES);
        return NULL;
    }
    
    return blockchain;
}


//* save blockchain from RAM to FILE

