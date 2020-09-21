#include "log.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char* log_file_path = "./player.log";

static char* get_timestamp();

void log(char* message) {
    FILE* log_file = fopen(log_file_path, "a");
    char* timestamp = get_timestamp();
    fprintf(log_file, "[%s] %s\n", timestamp, message);
    free(timestamp);
    fclose(log_file);
}

static char* get_timestamp() {
    time_t raw_time;
    struct tm* time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);

    char* timestamp = malloc(sizeof(char) * 100);
    strftime(timestamp, sizeof(char) * 100, "%y-%m-%d-%H-%M-%S", time_info);

    return timestamp;
}