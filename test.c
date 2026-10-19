#include <stdio.h>
#include <strings.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

struct termios origin_termios; // for saving the original termios config

void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios);
};

void enable_raw() {
    tcgetattr(STDIN_FILENO, &origin_termios);
    atexit(restore_terminal);

    struct termios raw = origin_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
};

char *input(char *prefix_text, size_t max_size) {
    enable_raw();
    max_size = max_size == 0 ? 50 : max_size;
    printf("%s", prefix_text);
    fflush(stdout);

    size_t i = 0;
    char *buff = malloc(max_size + 1);
    if (!buff) {
        return NULL;
    }

    char char_buff;
    // detect overflow
    // save data
    // use escape char for movinf cursor and more (colors ? ctrl and command support....)
    while (read(STDIN_FILENO, &char_buff, 1) == 1 && char_buff != '\n') {
        if (char_buff == '\x1b') {
            char seq[3];
            
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;

            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A': 
                        printf("/\\\r\n"); 
                        break;
                    case 'B': 
                        printf("V\r\n"); 
                        break;
                    case 'C': 
                        printf("->\r\n"); 
                        break;
                    case 'D': 
                        printf("<-\r\n"); 
                        break;
                }
            }
        } else {
            printf("%c\r", char_buff);
            buff[i++] = char_buff;
        }
    }

    restore_terminal();
    return buff;
};

int main(void) {
    char *res = input("type text :", 100);
    printf("\noutput'%s'\n", res);
    
    return 0;
}