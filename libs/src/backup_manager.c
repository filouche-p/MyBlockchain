#include "../headers/utils.h"
#include "../headers/node_manager.h"
#include "../headers/block_manager.h"
#include "../headers/command_sup.h"


/*
    ASSUMING that the data read in backup file are 100% correct
*/
//* load blockchain from FILE to RAM


//* save blockchain from RAM to FILE
t_node *load_blockchain(const char *file_path) {
    t_node *node = NULL;

    int fd = open(file_path, O_RDONLY);
    if (fd < 0) return NULL;
        // no backup or very bad error (assuming bad error)

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

void save_blockchain(t_node **blockchain_address, char *file_name) {
    t_node *node = *blockchain_address;

    int fd = open(file_name, O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        printf("error fd %d\n", fd);
        print_error(1);
        return;
    } 

    while(node) {
        char *nid = base_to_str(node->nid, 10);
        write(fd, nid, my_strlen(nid));
        write(fd, " : ", 3);
        // check for write return

        if(node->first_block) {
            t_block *block = node->first_block;

            size_t i = 0;
            size_t block_size = block_len(blockchain_address, node->nid);
            
            while(i < block_size) {
                write(fd, block->bid, my_strlen(block->bid));
                if (i < block_size - 1) {
                    write(fd, "-", 1);
                }
                // check for write return
                block = block->next;
                i++;
            }
        }

        write(fd, "\n", 1);
        node = node->next;
    }

    close(fd);
}
