#ifndef UTILS_H
#define UTILS_H


void print_error(size_t error_code);
void *smalloc(size_t size);
size_t len(char *string);
size_t count_words(char *str, char sep);
char *extract_word(char *str, size_t start, size_t length);
char **split(char *string, char sep);
size_t get_file_size(char *filename);
char *input(char *printed_text, size_t max_size);
void free_split(char **split_array);
ssize_t _index(char **array, char *word);
bool in(char** array, char *word);
bool check_nid(const char *string);
bool check_bid(const char *_);
size_t array_len(char **array);

#endif