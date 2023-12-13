#ifndef function_h
#define function_h
#include <stdio.h>
#include <fcntl.h> //files
#include <stdlib.h> //malloc, srand, rand
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h> //pid_t
#include <signal.h> // kill
#include <time.h> //time(NULL)
#include <string.h>

#define MAX_LEN 255 // max length for file's names
#define SIGTERM 15

/// @brief 
void kill();
int inputing(char **output_name, int fd, int endl_status);
void pipe_creation(int *fd);
int process_creation();
bool string_changing(char **output_string, char* input_string, int len);
int probability();

#endif
