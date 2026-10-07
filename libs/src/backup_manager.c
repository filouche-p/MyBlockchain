#include "../headers/utils.h"
#include "../headers/node_manager.h"
#include "../headers/block_manager.h"


/*
    ASSUMING that the data read in backup file are 100% correct
*/
//* load blockchain from FILE to RAM
// t_node *load_blockchain_dep(const char *file_path) {
//     printf("deprecated use new load function\n");
//     // internal call we don't check for file path
//     size_t block_size_read = 50;
//     t_node *blockchain = NULL;

//     int fd = open(file_path, O_RDONLY);
//     if (fd < 0) {
//         // no backup or very bad error (assuming bad error)
//         return NULL;
//     }

//     // we read line by line, each line is a node, everything after ':' is a block each block separated by a space
//     char *buff = smalloc(2048);
//     if (!buff) {
//         print_error(NO_MORE_RESSOURCES);
//         return NULL;
//     }

//     char **current_node;
//     ssize_t bytes_read;

//     size_t y = 0;

//     // we read until read return 0 (eof)
//     // read one by one until we found \n save to variable one for the start one for the end read start from the end -> you got one line = one node, split on space ("split on ->"?) tou got block
//     while ((bytes_read = read(fd, buff, block_size_read)) > 0) {
//         printf("new read\n");
//         current_node = split(split(buff, '\n')[y], ' ');

//         int nid = str_to_int(current_node[0]);

//         add_node(&blockchain, nid);

//         size_t i = 1;
//         while (current_node[i]) {
//             printf("new block\n");
//             if (strcmp(current_node[i], ":") == 0) {
//                 i++;
//                 continue;
//             }

//             char *bid = my_strdup(current_node[i]);
//             add_block(&blockchain, nid, bid);
//             printf("block added : %d : %s\n", nid, bid);
//             i++;
//         }

//         y++;
//     }

//     if (bytes_read == -1) {
//         // error while reading
//         print_error(NO_MORE_RESSOURCES);
//         return NULL;
//     }
    
//     return blockchain;
// }



//* save blockchain from RAM to FILE
t_node *load_blockchain(const char *file_path) {
    t_node *node = NULL;

    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        // no backup or very bad error (assuming bad error)
        return NULL;
    }

    size_t start_node_index = 0; size_t end_node_index = 0; size_t char_pos = 0; ssize_t bytes_read; char small_buff; char *buff_node = NULL;

    while ((bytes_read = read(fd, &small_buff, 1)) > 0) {
        if (small_buff == '\n') {
            end_node_index = char_pos;

            if (end_node_index > start_node_index) {
                int fd_node = open(file_path, O_RDONLY);
                if (fd_node < 0) return NULL;

                move_cursor(fd_node, start_node_index);

                size_t len = end_node_index - start_node_index;
                buff_node = smalloc(len + 1);
                if (!buff_node) {
                    close(fd_node);
                    return NULL;
                }

                ssize_t n_read = read(fd_node, buff_node, len);
                close(fd_node);

                if (n_read <= 0) {
                    free(buff_node);
                    buff_node = NULL;
                    start_node_index = char_pos + 1;
                    char_pos++;
                    continue;
                }
                buff_node[n_read] = '\0';

                char **splited_node = split(buff_node, ' ');

                if (splited_node && splited_node[0] && splited_node[0][0] != '\0') {
                    int nid = str_to_int(splited_node[0]);
                    add_node(&node, nid);

                    size_t i = 1;
                    while (splited_node[i]) {
                        if (splited_node[i][0] != ':' && splited_node[i][0] != '\0') {
                            char *bid = my_strdup(splited_node[i]);
                            add_block(&node, nid, bid);
                        }
                        i++;
                    }
                }
            }

            start_node_index = char_pos + 1;
        }

        if(buff_node) {
            free(buff_node);
            buff_node = NULL;
        }

        char_pos++;
    }

    close(fd);

    return node;
}

t_node *save_blockchain(t_node **blockchain_address) {
    t_node *node = *blockchain_address;

    while(node) {

        if(node->first_block) {
            t_block *block = node->first_block;
            
            while(block) {
                block = block->next;
            }
        }

        node = node->next;
    }
}
