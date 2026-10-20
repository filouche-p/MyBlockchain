#include <stdio.h>
#include <strings.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#define HISTORY_SIZE 101
char *history[HISTORY_SIZE];
size_t HISTORY_INDEX = 0;

void clear_history() {
    for(size_t i = 0; i < HISTORY_SIZE; i++) {
        history[i] = NULL;
    }
}

void show_history(int show_null) {
    for(size_t i = 0; i < HISTORY_SIZE; i++) {
        if(!show_null && !history[i]) {
            continue;
        }
        printf("%s\n", history[i]);
    }
}

size_t get_last_index() {
    size_t i = HISTORY_SIZE - 1;
    while(!history[i] && i != 0) {
        i--;
    }
    return i;
}

struct termios origin_termios; // for saving the original termios config

void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios);
}

void enable_raw() {
    tcgetattr(STDIN_FILENO, &origin_termios);
    atexit(restore_terminal);

    struct termios raw = origin_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char *input(char *prefix_text, size_t max_size) {
    enable_raw();
    max_size = max_size == 0 ? 50 : max_size;
    printf("%s", prefix_text);
    fflush(stdout);

    size_t HISTORY_POS = 0;

    size_t i = 0;
    char *buff = malloc(max_size + 1);
    if (!buff) {
        return NULL;
    }

    char char_buff;
    // detect overflow
    // save data
    // use escape char for movinf cursor and more (suppr, colors ? ctrl and command support....)
    while(read(STDIN_FILENO, &char_buff, 1) == 1 && char_buff != '\n') {
        if (char_buff == '\x1b') {
            char seq[3];
            
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;

            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A': // up
                        HISTORY_POS = get_last_index();

                        // printf("INDEX : %lu\nPOS : %lu\n", HISTORY_INDEX, HISTORY_POS);
                        // printf("debug: %s\n", history[HISTORY_POS]);
                        return history[HISTORY_POS];
                    case 'B': // down
                        if (HISTORY_POS > 0) {
                            HISTORY_POS--;
                        }
                        return history[HISTORY_POS];
                    case 'C': // right
                        break;
                    case 'D': //left
                        break;
                }
            }
        } else {
            // printf("%c\r", char_buff);
            write(STDOUT_FILENO, &char_buff, 1);
            buff[i++] = char_buff;
        }
    }

    buff[i] = '\0'; 
    history[HISTORY_INDEX] = strdup(buff);
    HISTORY_INDEX = (HISTORY_INDEX + 1) % HISTORY_SIZE;

    restore_terminal();
    return buff;
};

int main(void) {
    clear_history();

    size_t i = 0;
    while (i < 10) {
        char *res = input("type text :", 100);
        printf("\noutput'%s'\n", res);
        i++;
    }

    return 0;
}