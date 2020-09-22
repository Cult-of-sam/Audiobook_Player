#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <exports/logging.h>

// Library globals
static char* file_path = NULL;

// Private function declarations
static char* get_timestamp();
static void log_generic(char* level, char* message);

// Exported function implementations
void log_debug(char* message) {
    log_generic("DBG", message);
}

void log_info(char* message) {
    log_generic("INF", message);
}

void log_error(char* message) {
    log_generic("ERR", message);
}

// set_log_file_path sets the file path for the logging library to use.
// Frees the previous value stored before setting the new value.
void set_log_file_path(char* path) {
    free(file_path);
    file_path = path;
}

// Private function implementations
static char* get_timestamp() {
    time_t raw_time;
    time(&raw_time);
    struct tm* time_info = localtime(&raw_time);

    char* timestamp = malloc(sizeof(char) * 100);
    strftime(timestamp, sizeof(char) * 100, "%y/%m/%d-%H:%M:%S", time_info);

    return timestamp;
}

static void log_generic(char* level, char* message) {
    if (file_path == NULL) {
        char default_file_path[] = "player.log";
        file_path = malloc(sizeof(default_file_path));
        strcpy(file_path, default_file_path);
    }

    FILE* log_file = fopen(file_path, "a");
    char* timestamp = get_timestamp();
    fprintf(log_file, "[%s] %s | %s\n", timestamp, level, message);
    free(timestamp);
    fclose(log_file);
}