#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void *smalloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "nok: 1\n");
        exit(1);
    }

    return ptr;
}

size_t len(char *string) {
    size_t i = 0;

    while (string[i] != '\0') { i++; }

    return i;
}

char **split(char *string, char sep) {
    size_t length = len(string);

    char **res = smalloc(length);
    char *sub_string = smalloc(length);
    
    size_t i = 0;
    size_t y = 0;
    
    while (string[i] != '\0') {
        if (string[i] == sep) {
            y = 0;
        } else {
            sub_string[y] = string[i];
        }
    }

}

void print_error(size_t error_code) {
    const char *errors[] = {
        "",
        "nok: no more resources available on the computer\n",
        "nok: this node already exists\n",
        "nok: this block already exists\n",
        "nok: node doesn't exist\n",
        "nok: block doesn't exist\n",
        "nok: command not found\n"
    };

    const int length[] = { 0, 49, 30, 31, 24, 25, 23};

    if (1 <= error_code && error_code <= 6) {
        write(2, errors[error_code], length[error_code]);
    } else {
        write(2, "nok: unknow error code\n", 23);
    }
}

size_t count_words(char *str, char sep) {
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

    size_t words = count_words(string, sep);
    char **res = smalloc((words + 1) * sizeof(char *));
    
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
        printf("Error while reading\n");
        close(fd);
        return 0;
    }

    close(fd);
    return total_size;
}