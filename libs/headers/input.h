#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <signal.h>
#include "utils.h"

typedef struct s_history {
    char **entries;
    size_t count;
    size_t capacity;
} t_history;

void history_add(t_history *hist, const char *line);
void history_free(t_history *hist);
char *custom_input(const char *prompt, size_t max_size, t_history *hist, char **completions);

#endif
