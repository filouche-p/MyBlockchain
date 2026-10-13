#ifndef UTILS_H
#define UTILS_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define NO_MORE_RESSOURCES 1
#define NODE_ALREADY_EXISTS 2
#define BLOCK_ALREADY_EXISTS 3
#define NODE_DOESNT_EXISTS 4
#define BLOCK_DOESNT_EXISTS 5
#define COMMAND_NOT_FOUND 6

typedef struct s_block {
    char *bid;
    struct s_block *next;
} t_block;

typedef struct s_node {
    int nid;
    t_block *first_block;
    struct s_node *next;
} t_node;

void print_error(size_t error_code);
void *smalloc(size_t size);
size_t my_strlen(const char *string);
size_t count_words(const char *str, const char sep);
char *extract_word(char *str, size_t start, size_t length);
char **split(char *string, char sep);
size_t get_file_size(char *filename);
char *input(char *printed_text, size_t max_size);
void free_array(char **array);
void free_blockchain(t_node **blockchain_address);
bool check_nid(t_node **blockchain, const char *string);
bool check_bid(const char *_);
size_t array_len(char **array);
int my_strcmp(const char *s1, const char *s2);
bool can_be_int(const char *suppossed_number);
int str_to_int(const char *string);
char *my_strcpy(char *string1, const char *string2);
char *my_strdup(const char *string);
ssize_t my_index(const char *string, char element);
bool contains(const char *string, char element);
void move_cursor(int fd, ssize_t bytes_to_skip);
size_t int_len(unsigned int integer, int base);
char *base_to_str(int n, int base);
bool has_same_contents(char **array1, char **array2);
char **array_diff(char **array1, char **array2);

#endif