#include "../headers/utils.h"
#include "../headers/node_manager.h"
#include "../headers/block_manager.h"
#include "../headers/command_sup.h"


/*
    ASSUMING that the data read in backup file are 100% correct
*/
t_node *load_blockchain(const char *file_path) {
    t_node *node = NULL;

    int fd = open(file_path, O_RDONLY);
    if (fd < 0) return NULL;

    size_t start_node_index = 0; 
    size_t end_node_index = 0; 
    size_t char_pos = 0; 
    ssize_t bytes_read; 
    char small_buff; 
    char *buff_node = NULL;

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
                            char **blocs_separes = split(splited_node[i], '-');
                            
                            if (blocs_separes) {
                                size_t j = 0;
                                while (blocs_separes[j]) {
                                    if (blocs_separes[j][0] != '\0') {
                                        add_block(&node, nid, blocs_separes[j]);
                                    }
                                    j++;
                                }
                                free_array(blocs_separes);
                            }
                            
                        }
                        i++;
                    }
                }

                free_array(splited_node);
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

    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        print_error(NO_MORE_RESSOURCES);
        return;
    }


    while(node) {
        char *nid = base_to_str(node->nid, 10);
        size_t nid_len = my_strlen(nid);

        if (write(fd, nid, nid_len) != (ssize_t)nid_len) {
            free(nid);
            close(fd);
            return;
        }
        if (write(fd, " : ", 3) != 3) {
            free(nid);
            close(fd);
            return;
        }

        if(node->first_block) {
            t_block *block = node->first_block;

            size_t i = 0;
            size_t block_size = get_node_block_len(node);

            while(i < block_size) {
                size_t bid_len = my_strlen(block->bid);
                if (write(fd, block->bid, bid_len) != (ssize_t)bid_len) {
                    free(nid);
                    close(fd);
                    return;
                }
                if (i < block_size - 1) {
                    if (write(fd, "-", 1) != 1) {
                        free(nid);
                        close(fd);
                        return;
                    }
                }

                block = block->next;
                i++;
            }
        }

        if (write(fd, "\n", 1) != 1) {
            free(nid);
            close(fd);
            return;
        }
        node = node->next;
        free(nid);
    }

    close(fd);
}
