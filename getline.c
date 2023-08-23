#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream) {
    static char buffer[BUFFER_SIZE];
    static size_t buffer_pos = 0;
    static size_t buffer_size = 0;
    
    if (buffer_pos >= buffer_size) {
        buffer_size = read(fileno(stream), buffer, BUFFER_SIZE);
        if (buffer_size <= 0) {
            return -1; // EOF or error
        }
        buffer_pos = 0;
    }
    
    size_t line_length = 0;
    char *line_start = &buffer[buffer_pos];
    
    while (buffer_pos < buffer_size) {
        char c = buffer[buffer_pos++];
        
        if (c == '\n') {
            line_start[line_length] = '\0'; // Null-terminate the line
            line_length++;
            *lineptr = line_start;
            return line_length;
        }
        
        line_start[line_length++] = c;
        
        // Resize the buffer if necessary
        if (line_length + 1 >= *n) {
            *n *= 2;
            *lineptr = realloc(*lineptr, *n);
        }
    }
    
    // If the line doesn't fit in the current buffer, resize it
    *n = line_length + 1;
    *lineptr = realloc(*lineptr, *n);
    strncpy(*lineptr, line_start, line_length);
    
    return line_length;
}

int main() {
    char *line = NULL;
    size_t len = 0;
    
    while (1) {
        ssize_t n = custom_getline(&line, &len, stdin);
        
        if (n == -1) {
            printf("\nGoodbye!\n");
            break;
        }
        
        printf("Read %zd characters: %s\n", n, line);
    }
    
    free(line);
    return 0;
}

