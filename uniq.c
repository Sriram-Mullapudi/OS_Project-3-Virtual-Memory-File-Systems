#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_BUFFER 1024

// Check if input is provided via a pipe
int is_pipe_input() {
    struct stat stat_buf;
    fstat(0, &stat_buf);
    return stat_buf.type != T_DEV;
}

// Print the current line and store it as the previous one
void print_and_store(char *curr, char *prev) {
    printf(1, "%s\n", curr);
    strcpy(prev, curr);
}

// Compare two lines with optional width restriction
int compare_lines(char *a, char *b, int width) {
    if (width == 0) {
        while (*a && *b && *a == *b) {
            a++;
            b++;
        }
        return *a == *b;
    } else {
        for (int i = 0; i < width; i++) {
            if (*a != *b) return 0;
            if (!*a || !*b) break;
            a++;
            b++;
        }
        return 1;
    }
}

// Process and print lines based on the given flags
void process_output(char *curr, char *prev, int *count, int show_count, int show_unique) {
    if (!show_count && !show_unique) {
        printf(1, "%s\n", prev);
    } else if (show_count && *count > 0) {
        printf(1, "%d %s\n", *count, prev);
    } else if (show_unique && *count == 1) {
        printf(1, "%s\n", prev);
    }
    strcpy(prev, curr);
    *count = 1;
}

int main(int argc, char *argv[]) {
    int fd = 0;  // Default to stdin
    int show_count = 0, show_unique = 0, max_width = 0;
    int arg_idx = 1;

    // Parse command-line arguments
    while (arg_idx < argc) {
        if (argv[arg_idx][0] == '-') {
            switch (argv[arg_idx][1]) {
                case 'c':
                    show_count = 1;
                    break;
                case 'u':
                    show_unique = 1;
                    break;
                case 'w':
                    if (arg_idx + 1 < argc) max_width = atoi(argv[++arg_idx]);
                    break;
                default:
                    printf(1, "Usage: uniq [options] <filename>, valid options are -c, -u, -w [N]\n");
                    exit();
            }
        }
        arg_idx++;
    }

    // Determine input source: stdin or file
    if (argc == 1) {
        if (is_pipe_input()) {
            fd = 0;
        } else {
            printf(1, "Usage: uniq [options] <filename>\n");
            exit();
        }
    } else {
        if ((fd = open(argv[argc - 1], O_RDONLY)) < 0) {
            printf(1, "Error: Unable to open %s\n", argv[argc - 1]);
            exit();
        }
    }

    char buffer[MAX_BUFFER], last_line[MAX_BUFFER] = "";
    int line_count = 0, is_first_line = 1, bytes_read;

    // Read from input and process each line
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            char current_line[MAX_BUFFER];
            int char_count = 0;

            // Read a single line until a newline character is encountered
            while (i < bytes_read && buffer[i] != '\n') {
                current_line[char_count++] = buffer[i++];
            }
            current_line[char_count] = '\0';

            // Handle the first line separately
            if (is_first_line) {
                strcpy(last_line, current_line);
                line_count = 1;
                is_first_line = 0;
            } else if (compare_lines(last_line, current_line, max_width)) {
                line_count++;
            } else {
                process_output(current_line, last_line, &line_count, show_count, show_unique);
            }
        }
    }

    // Handle the final line in the input
    if (line_count > 0 && !is_first_line) {
        process_output("", last_line, &line_count, show_count, show_unique);
    }

    // Check for reading errors
    if (bytes_read < 0) {
        printf(1, "uniq: error reading the file\n");
    }

    close(fd);
    exit();
}
