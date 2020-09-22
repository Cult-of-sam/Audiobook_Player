#pragma once

void log_debug(char* message);
void log_info(char* message);
void log_error(char* message);

// set_log_file_path sets the file path for the logging library to use
// frees the previous value stored before setting the new value
void set_log_file_path(char* path);