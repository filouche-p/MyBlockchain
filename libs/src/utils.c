#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "../headers/utils.h"

void print_error(size_t error_code) {
    const char *errors[] = {
        "",
        "nok: no more resources available on the computer\n",
        "nok: this node already exists or the nid is not valid (only number are accepted).\n",
        "nok: this block already exists\n",
        "nok: node doesn't exist\n",
        "nok: block doesn't exist\n",
        "nok: command not found\n"
    };

    const int length[] = { 0, 49, 82, 31, 24, 25, 23};

    if (1 <= error_code && error_code <= 6) {
        write(2, errors[error_code], length[error_code]);
    } else {
        write(2, "nok: unknow error code\n", 23);
    }
}

void *my_memset(void *ptr, int value, size_t size) {
    unsigned char *p = (unsigned char *)ptr;

    for (size_t i = 0; i < size; i++) {
        *(p + i) = (unsigned char)value;
    }

    return ptr;
}

void *smalloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        print_error(NO_MORE_RESSOURCES);
        return NULL;
    }

    my_memset(ptr, 0, size);

    return ptr;
}

size_t my_strlen(const char *string) {
    if (string == NULL) {
        return 0;
    }

    size_t i = 0;

    while (string[i] != '\0') { i++; }

    return i;
}

size_t array_len(char **array) {
    size_t i = 0;

    while (array[i]) {
        i++;
    }

    return i;
}

size_t count_words(const char *str, const char sep) {
    size_t count = 0;
    size_t i = 0;

    while (str[i] != '\0') {
        while (str[i] == sep) {
            i++;
        }

        if (str[i] != '\0' && str[i] != sep) {
            count++;
            while (str[i] != '\0' && str[i] != sep) {
                i++;
            }
        }
    }

    return count;
}

char *extract_word(char *str, size_t start, size_t length) {
    char *word = smalloc(length + 1);
    if(!word) {
        return NULL;
    }

    size_t j = 0;
    
    while (j < length) {
        word[j] = str[start + j];
        j++;
    }

    word[j] = '\0';

    return word;
}

char **split(char *string, char sep) {
    if (string == NULL) return NULL;
    // sep = (unsigned char)sep;

    size_t words = count_words(string, sep);
    char **res = smalloc((words + 1) * sizeof(char *));
    if (!res) {
        print_error(NO_MORE_RESSOURCES);
        return NULL;
    }
    
    size_t i = 0;
    size_t word_idx = 0;

    while (string[i] != '\0') {
        while (string[i] == sep) i++;
        if (string[i] == '\0') break;

        size_t start = i;
        while (string[i] != '\0' && string[i] != sep) i++;
        
        res[word_idx] = extract_word(string, start, i - start);
        word_idx++;
    }
    
    res[word_idx] = NULL;
    return res;
}

size_t get_file_size(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Error while opening\n");
        return 0;
    }

    size_t total_size = 0;
    ssize_t bytes_read;
    size_t buffer_size = 4096;
    char buffer[buffer_size];

    while ((bytes_read = read(fd, buffer, buffer_size)) > 0) {
        total_size += bytes_read;
    }

    if (bytes_read == -1) {
        fprintf(stderr, "nok: no more resources available on the computer\n");
        close(fd);
        return 0;
    }

    close(fd);
    return total_size;
}

char *input(char *printed_text, size_t max_size) {
    if (max_size == 0) {
        // Default size
        max_size = 50;
    }

    printf("%s", printed_text);

    fflush(stdout);

    char *buff = smalloc(max_size + 1);
    if (!buff) {
        return NULL;
    }
    
    ssize_t bytes_read = read(0, buff, max_size);

    if (bytes_read <= 0) {
        if (bytes_read == -1) {
            print_error(NO_MORE_RESSOURCES);
        }

        free(buff);
        return NULL;
    }

    if (buff[bytes_read - 1] == '\n') {
        buff[bytes_read - 1] = '\0';
    } else {
        buff[bytes_read] = '\0';
    }

    return buff;
}

void free_split(char **split_array) {
    if (split_array == NULL) return;

    size_t i = 0;
    
    while (split_array[i] != NULL) {
        free(split_array[i]);
        i++;
    }
    
    free(split_array);
}

int my_strcmp(const char *s1, const char *s2) {
    if (!s1 && !s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;

    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    
    return (unsigned char)*s1 - (unsigned char)*s2;
}

bool can_be_int(const char *suppossed_number) {
    if (!suppossed_number || suppossed_number[0] == '\0') {
        return false;
    }
    
    size_t i = 0;
    while('0' <= *(suppossed_number + i) && *(suppossed_number + i) <= '9') {
        i++;
    }

    if (my_strlen(suppossed_number) != i) {
        return false;
    }

    return true;
}

bool check_nid(t_node **blockchain, const char *supposed_nid) {
    if (!can_be_int(supposed_nid)) {
        return false;
    } else if (*blockchain == NULL) {
        return true;
    }

    int valid_nid = str_to_int(supposed_nid);
    t_node *node = *blockchain;

    if(valid_nid == node->nid) {
        return false;
    }

    while(node->next != NULL) {
        if(valid_nid == node->nid) {
            return false;
        }
        node = node->next;
    }

    return true;

}

bool check_bid(const char *bid_str) {
    // print();
    printf("deprecated please make a full check bid\n");
    //! bid need to be unique in the specifed node
    if (!bid_str && bid_str[0] != '\0') {
        return false;
    }

    // any non-null string can be used as string
    return true;
}

int str_to_int(const char *string) {
    int res = 0;
    int sign = 1;

    if (!string) {
        print_error(NO_MORE_RESSOURCES);
        return 0;
    }

    if (*string == '-') {
        sign = -1;
        string++;
    }

    while (*string != '\0') {
        res = res * 10 + (*string - '0');
        string++;
    }

    return res * sign;
}

char *my_strcpy(char *string1, const char *string2) {
    size_t i = 0;
    while (string2[i]) {
        string1[i] = string2[i];
        i++;
    }

    string1[i] = '\0';

    return string1;
}

char *my_strdup(const char *string) {
    char *res = smalloc(my_strlen(string) + 1);
    
    if (!res) {
        print_error(NO_MORE_RESSOURCES);
        return NULL;
    }

    return my_strcpy(res, string);
}

ssize_t my_index(const char *string, char element) {
    size_t i = 0;

    while (string[i] != '\0') {
        if (string[i] == element) {
            return i;
        }
    }

    return -1;
}

bool contains(const char *string, char element) {
    return (bool)(my_index(string, element) + 1);
}

void move_cursor(int fd, ssize_t bytes_to_skip) {
    if (fd < 0) {
        return;
    }

    ssize_t to_read;
    ssize_t bytes_read;
    char trash_buff[1024];

    while (bytes_to_skip) {
        to_read = bytes_to_skip > 1024 ? 1024 : bytes_to_skip; // we read buff size or the rest

        bytes_read = read(fd, trash_buff, to_read);
        if (bytes_read == -1) { //* error or EOF
            break;
        }

        //*we cycle
        bytes_to_skip -= bytes_read;
    }
}

size_t int_len(unsigned int integer, int base) {
    size_t len = 0;

    do {
        len++;
        integer /= base;
    } while (integer != 0);

    return len;
}

char *base_to_str(int n, int base) {
    char *str;
    char *digits = "0123456789ABCDEF";
    if (base < 2 || base > 16) return NULL;

    int len = int_len(n, base);
    bool is_negative = n < 0;
    
    unsigned int un = is_negative ? -(unsigned int)n : (unsigned int)n;

    int total_len = len + (is_negative ? 1 : 0);
    
    str = malloc(sizeof(char) * (total_len + 1));
    if (!str) {
        return NULL;
    }

    str[len] = '\0';

    do {
        str[--total_len] = digits[un % base];
        un /= base;
    } while (un);

    if (is_negative) {
        str[0] = '-';
    }
    
    return str;
}