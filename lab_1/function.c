#include "function.h"

int inputing(char **s_output, int fd, int endl_status){
    int new_l=MAX_LEN;
    char *line=(char*)malloc(sizeof(char)*new_l); // выделяем память под line размером MAX_LEN = 255 байт
    memset(line, 0 , new_l); //заполняем line нулями
    int i=0;
    char ch; // выделили 1 байт, чтобы считывать STDIN_FILENO посимвольно
    read(fd, &ch, sizeof(ch));
    if(ch=='\n'){ // проверка на \n
        line[i]='\n';
        *s_output=line;
        return -1;
    }
    while(ch!=EOF && ch!='\0' && ch!='\n' ){ 
        if(i>=new_l){ // проверка не достигнута ли максимальная длина строки
            new_l=new_l*2;
            line=(char *)realloc(line, sizeof(char)*new_l); // увеличиваем объем выделенной памяти
        }
        line[i]=ch;
        i++;
        read(fd, &ch, sizeof(ch));  // продолжаем посимвольное считывание

    }
    if(endl_status!=0){ // если нужно вводить строку НЕ один раз
        if(i>=new_l){
            new_l=new_l*2;
            line=(char *)realloc(line, sizeof(char)*new_l);
        }
        line[i]='\n';
        i++;
    }
    if(i>=new_l){
            new_l=new_l*2;
            line=(char *)realloc(line, sizeof(char)*new_l);
        }
    line[i] = '\0';
    *s_output=line;
    return i;   
}

void kill()
{
}

void pipe_creation(int *fd){
    if (pipe(fd) == -1){
        perror("Call pipe was ended with error: ");
        exit(-1);
    }
}

int process_creation(){
    pid_t pid = fork();
    if (pid == -1){
        perror("Call fork was ended with erorr: ");
        exit(-1);
    }
    return pid;
}

int probability(){
    srand(time(NULL)); //инициализация генератора случайных чисел и установка текущего времени в качестве его базы
    int a =rand()%10+1; //случайные числа от 1 до 10
    if(a<=8){
        return 1;
    } else{
        return 2;
    }
}

bool string_changing(char **output_string, char* input_string, int len){ //fixed
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
