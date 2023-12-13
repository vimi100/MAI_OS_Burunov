#include "function.h"

int main(){
    while(true){
        char *input_strint=NULL;
        int s_len=inputing(&input_strint, STDIN_FILENO, 0);

        char* output_string=NULL;

        if ((input_strint[0]=='-')){
            continue;
        } else if(s_len<=0){
            free(input_strint);
            break;
        } else{
            if(string_changing(&output_string, input_strint, s_len)==0){
                write(STDOUT_FILENO, " String_invert Error2! ", 24);
                break;
            } else{
                write(STDOUT_FILENO, output_string, s_len*sizeof(char));
            }
        }
        free(input_strint);
    }
    return 0;
}
