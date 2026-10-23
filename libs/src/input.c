#include "../headers/input.h"
#include "../headers/utils.h"

typedef enum {
    KEY_NONE = 0,
    KEY_HOME = 1,
    KEY_EOF = 4,
    KEY_END = 5,
    KEY_TAB = 9,
    KEY_ENTER = 10,
    KEY_KILL_END = 11,
    KEY_CLEAR_SCREEN = 12,
    KEY_SEARCH = 18,
    KEY_CLEAR_LINE = 21,
    KEY_PASTE = 22,
    KEY_DEL_WORD = 23,
    KEY_BACKSPACE = 127,
    KEY_UP = 1000,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DELETE,
    KEY_WORD_LEFT,
    KEY_WORD_RIGHT,
    KEY_COPY
} t_key;

typedef struct s_hist_nav {
    t_history *hist;
    size_t index;
    char saved[256];
} t_hist_nav;

static struct termios g_orig;

static void sigint_handler(int sig) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_orig);
    write(STDOUT_FILENO, "\n", 1);
    signal(sig, SIG_DFL);
    raise(sig);
}

static int enable_raw_mode(struct termios *orig) {
    if (!isatty(STDIN_FILENO)) {
        return -1;
    }
    if (tcgetattr(STDIN_FILENO, orig) == -1) {
        return -1;
    }
    g_orig = *orig;
    signal(SIGINT, sigint_handler);
    struct termios raw = *orig;
    raw.c_lflag &= ~(ICANON | ECHO);
    return tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static void disable_raw_mode(const struct termios *orig) {
    signal(SIGINT, SIG_DFL);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig);
}

static int parse_escape_seq(void) {
    char seq[16];
    int n = 0;

    if (read(STDIN_FILENO, &seq[0], 1) <= 0) {
        return '\033';
    }
    if (read(STDIN_FILENO, &seq[1], 1) <= 0) {
        return '\033';
    }
    if (seq[0] == '[') {
        if (seq[1] >= 'A' && seq[1] <= 'D') {
            return KEY_UP + (seq[1] - 'A');
        }
        if (seq[1] == 'H') {
            return KEY_HOME;
        }
        if (seq[1] == 'F') {
            return KEY_END;
        }
        n = 2;
        while (n < 15 && read(STDIN_FILENO, &seq[n], 1) > 0) {
            if ((seq[n] >= 'A' && seq[n] <= 'Z') || (seq[n] >= 'a' && seq[n] <= 'z') || seq[n] == '~') {
                break;
            }
            n++;
        }
        seq[n + 1] = '\0';
        if (my_strcmp(&seq[1], "1;5D") == 0) {
            return KEY_WORD_LEFT;
        }
        if (my_strcmp(&seq[1], "1;5C") == 0) {
            return KEY_WORD_RIGHT;
        }
        if (my_strcmp(&seq[1], "3~") == 0) {
            return KEY_DELETE;
        }
        if (my_strstr(seq, ";6u") || my_strstr(seq, ";6;")) {
            return KEY_COPY;
        }
    }
    return '\033';
}

static int read_key(void) {
    char c;

    if (read(STDIN_FILENO, &c, 1) <= 0) {
        return KEY_EOF;
    }
    if (c == '\033') {
        return parse_escape_seq();
    }
    if (c == '\r') {
        return KEY_ENTER;
    }
    if (c == 8) {
        return KEY_BACKSPACE;
    }
    return (unsigned char)c;
}

static void refresh_line(const char *prompt, const char *buf, size_t pos) {
    size_t len = my_strlen(buf);

    printf("\r%s%s\033[K", prompt, buf);
    if (len > pos) {
        printf("\033[%zuD", len - pos);
    }
    fflush(stdout);
}

void history_add(t_history *hist, const char *line) {
    if (!line || !*line || !hist) {
        return;
    }
    if (hist->count > 0 && my_strcmp(hist->entries[hist->count - 1], line) == 0) {
        return;
    }
    if (hist->count >= hist->capacity) {
        size_t new_cap = hist->capacity == 0 ? 16 : hist->capacity * 2;
        size_t old_size = hist->capacity * sizeof(char *);
        size_t new_size = new_cap * sizeof(char *);
        char **new_entries = my_realloc(hist->entries, old_size, new_size);
        if (!new_entries) {
            return;
        }
        hist->entries = new_entries;
        hist->capacity = new_cap;
    }
    hist->entries[hist->count++] = my_strdup(line);
}

void history_free(t_history *hist) {
    if (!hist || !hist->entries) {
        return;
    }
    for (size_t i = 0; i < hist->count; i++) {
        free(hist->entries[i]);
    }
    free(hist->entries);
    hist->entries = NULL;
    hist->count = 0;
    hist->capacity = 0;
}

static void handle_history(int key, char *buf, size_t *pos, size_t *len, t_hist_nav *nav, size_t max_size) {
    if (!nav->hist || nav->hist->count == 0) {
        return;
    }
    if (key == KEY_UP && nav->index == nav->hist->count) {
        my_strncpy(nav->saved, buf, sizeof(nav->saved));
    }
    if (key == KEY_UP && nav->index > 0) {
        nav->index--;
    } else if (key == KEY_DOWN && nav->index < nav->hist->count) {
        nav->index++;
    } else if (key == KEY_DOWN) {
        return;
    }
    char *src = (nav->index == nav->hist->count) ? nav->saved : nav->hist->entries[nav->index];
    my_strncpy(buf, src, max_size);
    buf[max_size] = '\0';
    *len = my_strlen(buf);
    *pos = *len;
}

static void handle_tab(char *buf, size_t *pos, size_t *len, char **completions, size_t max_size, const char *prompt) {
    if (!completions) {
        return;
    }
    int count = 0;
    int match_idx = -1;
    for (int i = 0; completions[i]; i++) {
        if (my_strncmp(completions[i], buf, *pos) == 0) {
            count++;
            match_idx = i;
        }
    }
    if (count == 1) {
        my_strncpy(buf, completions[match_idx], max_size);
        buf[max_size] = '\0';
        *len = my_strlen(buf);
        *pos = *len;
    } else if (count > 1) {
        printf("\n");
        for (int i = 0; completions[i]; i++) {
            if (my_strncmp(completions[i], buf, *pos) == 0) {
                printf("%s  ", completions[i]);
            }
        }
        printf("\n");
        refresh_line(prompt, buf, *pos);
    }
}

static void delete_word_backward(char *buf, size_t *pos, size_t *len) {
    size_t old_pos = *pos;

    while (*pos > 0 && buf[*pos - 1] == ' ') {
        (*pos)--;
    }
    while (*pos > 0 && buf[*pos - 1] != ' ') {
        (*pos)--;
    }
    size_t diff = old_pos - *pos;
    if (diff > 0) {
        my_memmove(&buf[*pos], &buf[old_pos], *len - old_pos + 1);
        *len -= diff;
    }
}

static void handle_shortcuts(int key, char *buf, size_t *pos, size_t *len, const char *prompt) {
    if (key == KEY_HOME) {
        *pos = 0;
    } else if (key == KEY_END) {
        *pos = *len;
    } else if (key == KEY_CLEAR_LINE) {
        buf[0] = '\0';
        *pos = 0;
        *len = 0;
    } else if (key == KEY_KILL_END) {
        buf[*pos] = '\0';
        *len = *pos;
    } else if (key == KEY_DEL_WORD) {
        delete_word_backward(buf, pos, len);
    } else if (key == KEY_CLEAR_SCREEN) {
        printf("\033[H\033[2J");
        refresh_line(prompt, buf, *pos);
    }
}

static void move_word_cursor(int key, const char *buf, size_t *pos, size_t len) {
    if (key == KEY_WORD_LEFT) {
        while (*pos > 0 && buf[*pos - 1] == ' ') {
            (*pos)--;
        }
        while (*pos > 0 && buf[*pos - 1] != ' ') {
            (*pos)--;
        }
    } else if (key == KEY_WORD_RIGHT) {
        while (*pos < len && buf[*pos] != ' ') {
            (*pos)++;
        }
        while (*pos < len && buf[*pos] == ' ') {
            (*pos)++;
        }
    }
}

static void handle_edit_key(int key, char *buf, size_t *pos, size_t *len, size_t max_size) {
    if (key == KEY_LEFT && *pos > 0) {
        (*pos)--;
    } else if (key == KEY_RIGHT && *pos < *len) {
        (*pos)++;
    } else if (key == KEY_WORD_LEFT || key == KEY_WORD_RIGHT) {
        move_word_cursor(key, buf, pos, *len);
    } else if (key == KEY_BACKSPACE && *pos > 0) {
        my_memmove(&buf[*pos - 1], &buf[*pos], *len - *pos + 1);
        (*pos)--;
        (*len)--;
    } else if (key == KEY_DELETE && *pos < *len) {
        my_memmove(&buf[*pos], &buf[*pos + 1], *len - *pos);
        (*len)--;
    } else if (key >= 32 && key <= 126 && *len < max_size) {
        my_memmove(&buf[*pos + 1], &buf[*pos], *len - *pos + 1);
        buf[*pos] = (char)key;
        (*pos)++;
        (*len)++;
    }
}

static void clipboard_copy(const char *str) {
    FILE *fp = popen("xclip -selection clipboard 2>/dev/null || wl-copy 2>/dev/null", "w");
    if (!fp) {
        return;
    }
    fputs(str, fp);
    pclose(fp);
}

static void clipboard_paste(char *buf, size_t *pos, size_t *len, size_t max_size) {
    FILE *fp = popen("xclip -selection clipboard -o 2>/dev/null || wl-paste 2>/dev/null", "r");
    if (!fp) {
        return;
    }
    char chunk[256];
    size_t n = fread(chunk, 1, sizeof(chunk) - 1, fp);
    pclose(fp);
    chunk[n] = '\0';
    for (size_t i = 0; i < n && *len < max_size; i++) {
        if (chunk[i] != '\r' && chunk[i] != '\n') {
            my_memmove(&buf[*pos + 1], &buf[*pos], *len - *pos + 1);
            buf[*pos] = chunk[i];
            (*pos)++;
            (*len)++;
        }
    }
}

static bool find_match(t_history *hist, const char *query, size_t max_idx, size_t *found_idx) {
    if (!*query || !hist) {
        return false;
    }
    for (size_t i = max_idx; i > 0; i--) {
        if (my_strstr(hist->entries[i - 1], query)) {
            *found_idx = i - 1;
            return true;
        }
    }
    return false;
}

static void handle_reverse_search(char *buf, size_t *pos, size_t *len, t_history *hist, size_t max_size) {
    char q[64] = {0};
    size_t qlen = 0;
    size_t idx = 0;
    bool found = false;

    while (true) {
        printf("\r(reverse-i-search)`%s': %s\033[K", q, found ? hist->entries[idx] : "");
        fflush(stdout);
        int key = read_key();
        if (key == KEY_ENTER) {
            if (found) {
                my_strncpy(buf, hist->entries[idx], max_size);
                *pos = my_strlen(buf);
                *len = *pos;
            }
            break;
        }
        if (key == '\033' || key == 7) {
            break;
        }
        if (key == KEY_SEARCH && found && idx > 0) {
            found = find_match(hist, q, idx, &idx);
        } else if (key == KEY_BACKSPACE && qlen > 0) {
            q[--qlen] = '\0';
        } else if (key >= 32 && key <= 126 && qlen < sizeof(q) - 1) {
            q[qlen++] = (char)key;
            q[qlen] = '\0';
        } else if (key < 32 || key >= 1000) {
            break;
        }
        if (key != KEY_SEARCH) {
            found = find_match(hist, q, hist ? hist->count : 0, &idx);
        }
    }
}

static void handle_key_action(int key, char *buf, size_t *pos, size_t *len, size_t max, const char *prompt) {
    if (key == KEY_COPY) {
        clipboard_copy(buf);
    } else if (key == KEY_PASTE) {
        clipboard_paste(buf, pos, len, max);
    } else if (key == KEY_HOME || key == KEY_END || key == KEY_CLEAR_LINE || key == KEY_CLEAR_SCREEN || key == KEY_DEL_WORD || key == KEY_KILL_END) {
        handle_shortcuts(key, buf, pos, len, prompt);
    } else {
        handle_edit_key(key, buf, pos, len, max);
    }
}

static char *read_fallback(const char *prompt, size_t max_size) {
    printf("%s", prompt);
    fflush(stdout);
    char *buf = smalloc(max_size + 1);
    if (!buf) {
        return NULL;
    }
    ssize_t n = read(STDIN_FILENO, buf, max_size);
    if (n <= 0) {
        free(buf);
        return NULL;
    }
    if (n > 0 && buf[n - 1] == '\n') {
        n--;
    }
    buf[n] = '\0';
    return buf;
}

char *custom_input(const char *prompt, size_t max_size, t_history *hist, char **completions) {
    struct termios orig;

    if (enable_raw_mode(&orig) == -1) {
        return read_fallback(prompt, max_size);
    }
    char *buf = smalloc(max_size + 1);
    if (!buf) {
        return NULL;
    }
    size_t pos = 0;
    size_t len = 0;
    t_hist_nav nav = {hist, hist ? hist->count : 0, {0}};
    refresh_line(prompt, buf, pos);
    while (true) {
        int key = read_key();
        if (key == KEY_ENTER) {
            break;
        }
        if (key == KEY_EOF) {
            free(buf);
            disable_raw_mode(&orig);
            return NULL;
        }
        if (key == KEY_SEARCH) {
            handle_reverse_search(buf, &pos, &len, hist, max_size);
        } else if (key == KEY_UP || key == KEY_DOWN) {
            handle_history(key, buf, &pos, &len, &nav, max_size);
        } else if (key == KEY_TAB) {
            handle_tab(buf, &pos, &len, completions, max_size, prompt);
        } else {
            handle_key_action(key, buf, &pos, &len, max_size, prompt);
        }
        refresh_line(prompt, buf, pos);
    }
    printf("\n");
    disable_raw_mode(&orig);
    return buf;
}
