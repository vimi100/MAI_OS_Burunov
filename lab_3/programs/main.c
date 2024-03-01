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

char* inputing() { // fixed inputing
    int len = 0;
    int capacity = 10; // Начальная емкость 
    char *s = (char*) malloc(capacity * sizeof(char)); // Выделяем начальную память
    if (s == NULL) {
        return NULL; // Проверка выделения памяти
    }
    char c = getchar();
    while (c != '\n') {
        if (len >= capacity - 1) { // Проверка, нужно ли увеличить буфер
            capacity *= 2;
            char* temp = (char*) realloc(s, capacity * sizeof(char)); // Пытаемся увеличить буфер
            if (temp == NULL) { // Проверка удачного realloc
                free(s);
                return NULL;
            }
            s = temp;
        }
        s[len++] = c;
        c = getchar();
    }
    s[len] = '\0';
    return s;
}

int process_creation(){
    pid_t pid = fork();
    if (pid == -1){
        perror("Call fork was ended with erorr: ");
        exit(EXIT_FAILURE);
    }
    return pid;
}

int probability(){ // from lab1
    srand(time(NULL)); //инициализация генератора случайных чисел и установка текущего времени в качестве его базы
    int a =rand()%10+1; //случайные числа от 1 до 10
    if(a<=8){
        return 1;
    } else{
        return 2;
    }
}

int main() {
    char *args1[] = {"child", "123",  NULL};
    char *args2[] = {"child", "456",  NULL};

    write(STDOUT_FILENO, "Enter the first filename with file extension(.txt or .doc or .rtf): ", 68);

    char * Filename_1 = NULL;
    Filename_1 = inputing();

    if (Filename_1 == NULL) {
        perror ("Inputing error:");
        exit(EXIT_FAILURE);
    }

    int f1_output = open(Filename_1, O_WRONLY| O_CREAT | O_TRUNC , S_IWUSR);

    free(Filename_1);


    if (f1_output == -1) {
        perror ("Can't open the file:");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "Enter the second filename with file extension(.txt or .doc or .rtf): ", 69);

    char *Filename_2 = NULL;
    Filename_2 = inputing();

    if (Filename_2 == NULL) {
        perror ("Inputing error:");
        exit(EXIT_FAILURE);
    }

    int f2_output = open(Filename_2, O_WRONLY| O_CREAT | O_TRUNC, S_IWUSR);

    free(Filename_2);

    if (f2_output == -1) {
        perror("Can't open the file:");
        exit(EXIT_FAILURE);
    }

    int fd1 = shm_open(
        MEMORY_NAME1, 
        O_CREAT | O_RDWR | O_TRUNC, 
        0777);
    ftruncate(fd1 , 1000*sizeof(char)); 
    char *addr1 = mmap(NULL, 1000*sizeof(char), PROT_WRITE | PROT_READ , MAP_SHARED ,fd1,0);

    if (addr1 == (void*)-1){
            perror("mmap1 error: ");
            exit(EXIT_FAILURE);
        }

    int fd2 = shm_open(
        MEMORY_NAME2, 
        O_CREAT | O_RDWR | O_TRUNC, 
        0777);
    ftruncate (fd2 , 1000*sizeof(char)); 
    char *addr2 = mmap(NULL, 1000*sizeof(char), PROT_WRITE | PROT_READ , MAP_SHARED ,fd2,0);

    if (addr2 == (void*)-1){
            perror("mmap2 error: ");
            exit(EXIT_FAILURE);
        }

    pid_t pid_1 = process_creation();
    if (pid_1 == 0)
    {   // the 1st child
        if (dup2(f1_output, STDOUT_FILENO) == -1){
            perror ("dup2 erorr: ");
            exit(EXIT_FAILURE);
        }
        if(execv("./child", args1) == -1){
            perror("execv1 erorr: ");
            exit(EXIT_FAILURE);
        }
    }else
    { 	//parent
    pid_t pid_2 = process_creation();
    if (pid_2 == 0)
    {   // the 2st child
        if (dup2(f2_output, STDOUT_FILENO) == -1){ 
            perror ("dup2 erorr: ");
            exit(EXIT_FAILURE);
        }
        if(execv("./child", args2) == -1){
            perror("execv2 erorr: ");
            exit(EXIT_FAILURE);
        }
    }else
    {	//parent
        char symbol;
        int iterator_1 = 0;
        int iterator_2 = 0;
        while(true){
                int prob_res=probability();
                if ((iterator_1 == 1000*sizeof(char)) || (iterator_2 == 1000*sizeof(char))){
                    perror("increase the size of the memory mapped file (files): ");
                    exit(EXIT_FAILURE);
                }
                while(((symbol = getchar()) != EOF) && (iterator_1 != 1000*sizeof(char)) && (iterator_2 != 1000*sizeof(char))){
                    if (prob_res==1){			
                        addr1[iterator_1] = (char)symbol;
                        iterator_1 ++;
                    } else {
                        addr2[iterator_2] = (char)symbol;
                        iterator_2 ++;
                    }
                    if (symbol == '\n'){	
                        if (prob_res==1){
                            kill(pid_1, SIGUSR1);
                            break;
                        }
                        else{
                            kill(pid_2, SIGUSR1);
                            break;
                        }
                    }		
                }
                if (symbol == EOF){
                    break;
                }
            }
        kill(pid_1, SIGUSR2);
        kill(pid_2, SIGUSR2);
        close(fd1);
        close(fd2);
        close(f1_output);
        close(f2_output);
        if(shm_unlink(MEMORY_NAME1) == -1){
            perror("shm_unlink(MEMORY_NAME1) error: ");
            exit(EXIT_FAILURE);
        }
        if(shm_unlink(MEMORY_NAME2) == -1){
            perror("shm_unlink(MEMORY_NAME2) error: ");
            exit(EXIT_FAILURE);
        }
        if(munmap(addr1, 1000*sizeof(char)) == -1){
            perror("munmap1 error:");
            exit(EXIT_FAILURE);
           }
        if(munmap(addr2, 1000*sizeof(char)) == -1){
            perror("munmap2 error:");
            exit(EXIT_FAILURE);
           }
        }
}
}