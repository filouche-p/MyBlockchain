#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#define HISTORY_SIZE 101
char *history[HISTORY_SIZE];

void clear_history() {
    for(size_t i = 0; i < HISTORY_SIZE; i++) {
        if (history[i] != NULL) {
            free(history[i]);
            history[i] = NULL;
        }
    }
}

struct termios origin_termios;

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

void push_history(const char *cmd) {
    if (!cmd || strlen(cmd) == 0) return;
    
    if (history[HISTORY_SIZE - 1]) {
        free(history[HISTORY_SIZE - 1]);
    }
    
    for (size_t i = HISTORY_SIZE - 1; i > 1; i--) {
        history[i] = history[i - 1];
    }
    
    history[1] = strdup(cmd);
}

char *input(char *prefix_text, size_t max_size) {
    enable_raw();
    max_size = max_size == 0 ? 50 : max_size;
    printf("%s", prefix_text);
    fflush(stdout);

    size_t HISTORY_POS = 0;
    size_t cursor = 0;
    size_t len = 0;
    char *buff = malloc(max_size + 1);
    if (!buff) {
        restore_terminal();
        return NULL;
    }
    buff[0] = '\0';

    if (history[0]) free(history[0]);
    history[0] = strdup("");

    char char_buff;
    
    while(read(STDIN_FILENO, &char_buff, 1) == 1 && char_buff != '\n') {
        if (char_buff == '\x1b') {
            char seq[3];
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;

            if (seq[0] == '[') {
                int history_changed = 0;

                if (seq[1] == 'A') { // Up
                    if (HISTORY_POS == 0) {
                        buff[len] = '\0';
                        free(history[0]);
                        history[0] = strdup(buff);
                    }
                    if (HISTORY_POS + 1 < HISTORY_SIZE && history[HISTORY_POS + 1] != NULL) {
                        HISTORY_POS++;
                        history_changed = 1;
                    }
                } 
                else if (seq[1] == 'B') { // Down
                    if (HISTORY_POS > 0) {
                        HISTORY_POS--;
                        history_changed = 1;
                    }
                }
                else if (seq[1] == 'C') { // Right
                    if (cursor < len) {
                        cursor++;
                        printf("\x1b[1C");
                        fflush(stdout);
                    }
                }
                else if (seq[1] == 'D') { // Left
                    if (cursor > 0) {
                        cursor--;
                        printf("\x1b[1D");
                        fflush(stdout);
                    }
                }

                if (history_changed && history[HISTORY_POS]) {
                    printf("\r\x1b[2K%s%s", prefix_text, history[HISTORY_POS]);
                    fflush(stdout);
                    
                    strncpy(buff, history[HISTORY_POS], max_size);
                    buff[max_size] = '\0';
                    len = strlen(buff);
                    cursor = len;
                }
            }
        } 
        else if (char_buff == 127 || char_buff == '\b') {
            if (cursor > 0) {
                memmove(&buff[cursor - 1], &buff[cursor], len - cursor);
                cursor--;
                len--;
                buff[len] = '\0';
                
                printf("\r\x1b[2K%s%s", prefix_text, buff);
                if (cursor < len) {
                    printf("\x1b[%zuD", len - cursor);
                }
                fflush(stdout);
            }
        } else if (char_buff == '\t') { // Tab
            const char *commands[] = {"clear", "exit", "help", "history"};
            int num_commands = 4;

            for (int k = 0; k < num_commands; k++) {
                if (len > 0 && strncmp(commands[k], buff, len) == 0) {
                    
                    strcpy(buff, commands[k]);
                    len = strlen(buff);
                    cursor = len;
                    
                    printf("\r\x1b[2K%s%s", prefix_text, buff);
                    fflush(stdout);
                    
                    break; 
                }
            }
        }
        else {
            if (len < max_size) {
                memmove(&buff[cursor + 1], &buff[cursor], len - cursor);
                buff[cursor] = char_buff;
                cursor++;
                len++;
                buff[len] = '\0';
                
                printf("\r\x1b[2K%s%s", prefix_text, buff);
                if (cursor < len) {
                    printf("\x1b[%zuD", len - cursor); 
                }
                fflush(stdout);
            }
        }
    }

    buff[len] = '\0';
    printf("\r\n");
    
    push_history(buff);

    restore_terminal();
    return buff;
}

int main(void) {
    clear_history();

    while (1) {
        char *res = input("type text : ", 100);
        printf("output: '%s'\n\n", res);
        if(!strcmp(res, "quit")) {
            break;
        }
        free(res);
    }

    clear_history();
    return 0;
}