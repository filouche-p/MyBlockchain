#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//malloc, free, printf, write, open, read, close

void *smalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        // voir coent remonter l'erreur
        // Crée une fonction throw error
        return NULL;
    }

    return ptr;
}

char *input(char *printed_text, size_t max_size) {
    if (max_size == 0) {
        max_size = 50; // default value
    }
    printf("%s", printed_text);
    char *buff = smalloc(max_size);
    
    ssize_t bytes_read = read(0, buff, max_size);

    if (bytes_read == -1) {
        printf("Unable to read system error exit\n");
        exit(0);
    } else if (bytes_read == 0) {
        printf("Invalid input / error\n");
        exit(0);
    }

    buff[bytes_read] = '\0';

    return buff;
}

// chemin d'accès dynamique ?
int save_blockchain() { 
    char *filename = ".bloackchain.save";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 644);

    if(fd == -1) {
        // throw error
        printf("Error while reading\n");
        return 0;
    }
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

char *read_blockchain() {
    char *filename = ".bloackchain.save";
    int fd = open(filename, O_RDONLY);
    if(fd == -1) {
        // throw error
        printf("Error while reading\n");
        return 0;
    }

    size_t size = get_file_size(filename);
    if (size == 0) {
        return -1;
    }

    char *data = smalloc(size);
    read(fd, &data, size);

    close(fd);

    return data;
}

char **split(char *string, char sep) {
    size_t length = strlen(string);

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

int rtos(char *raw_data) {
    // raw to struct
    // chaque ligne est un node
    // le nid est les premiers caractères de chaque ligne jusqu'au :
    // les : peuvent être ignorer
    // chaque blockid sont séparer par des , (potentiellement des espaces)

    char **rows = split(raw_data, '\n');

    return 0;
}

int main(void) {
    // Garbage collector ?????
    char *res = input("Entrez du texte >\n", 100);

    printf("%s", res);

    free(res);
    return 0;
}