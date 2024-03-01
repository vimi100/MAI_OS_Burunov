#include <stdio.h>
#include <fcntl.h> //files
#include <stdlib.h> //malloc, srand, rand
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h> //pid_t, ftruncate
#include <signal.h> // kill
#include <time.h> //time(NULL)
#include <sys/mman.h>
#include "stddef.h"
#include <string.h>
#include <sys/stat.h>

#define MEMORY_NAME1 "fd_virt1"
#define MEMORY_NAME2 "fd_virt2"

char *addr_global;
int flag = 0;
int i_global = 0;
    
bool vowel_del(char **output_string, char* input_string, int len){ // from lab1
    char tmp[len+1]; 
    for(int i=0; i<len;++i){
        if ((input_string[i] == 'a' || input_string[i] == 'e' || input_string[i] == 'i' || input_string[i] == 'o' || input_string[i] == 'u' 
        || input_string[i] == 'y' || input_string[i] == 'A' || input_string[i] == 'E' || input_string[i] == 'I' 
        || input_string[i] == 'O' || input_string[i] == 'U' || input_string[i] == 'Y')){
            tmp[i]=' ';    
        }else {
            tmp[i] = input_string[i];
        }
    }
    tmp[len]='\0';
    free(*output_string);
    *output_string=tmp;
    return true;
}

void writer() {
    char c;
    char* output_string = NULL;
    char* input_string = NULL;
    int input_string_size = 0; 

    for (int i = i_global; i < 1000 * sizeof(char); i++) {
        c = addr_global[i];
        input_string = realloc(input_string, (input_string_size + 2) * sizeof(char)); 

        if ((c != '\n') || (flag != 0)) {
            input_string[input_string_size] = c; 
            input_string[input_string_size + 1] = '\0';
            input_string_size++;
        }
        if (c == '\n') {
            flag++;
            i_global = i + 1;
            break;
            }   
    }
    if (vowel_del(&output_string, input_string, strlen(input_string)) == 0) {
        write(STDOUT_FILENO, "vowel_del Error! ", 17);
    } else {
        write(STDOUT_FILENO, output_string, input_string_size * sizeof(char));
    }
    free(input_string);
}

void finish(){
    if(munmap(addr_global, 1000*sizeof(char)) == -1){
        perror("munmap(child) error:");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, const char *argv[]){

    int arg = atoi(argv[1]);
    int shm_fd = 0;

    if (arg == 123) {
        shm_fd = shm_open(
            MEMORY_NAME1, 
            O_RDWR, 
            S_IRUSR);
    }else{
        shm_fd = shm_open(
            MEMORY_NAME2, 
            O_RDWR, 
            S_IRUSR);
    }
    char *addr = mmap(NULL, 1000*sizeof(char),  PROT_WRITE | PROT_READ , MAP_SHARED , shm_fd, 0);
    if (addr == (void*)-1){
            perror("mmap (child) error: ");
            exit(EXIT_FAILURE);
        }

    addr_global = addr;

    while (true)
    {
    signal (SIGUSR1, writer);
    signal (SIGUSR2, finish);
    }
}
