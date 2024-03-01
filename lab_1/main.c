#include "function.h"

int main(){

    write(STDOUT_FILENO, "Enter the first filename with file extension(.txt or .doc or .rtf): ", 67);

    char *Filename_1=NULL;
    char *Filename_2=NULL;

    if(inputing(&Filename_1 ,STDIN_FILENO, 0)<=0){
        perror("Trying to create 0-value string: ");
        exit(-1);
    }

    int f1_output=open(Filename_1, O_WRONLY | O_CREAT, 0777);
    int f2_output = 0;
    free(Filename_1);
    if(f1_output==-1){
        fprintf(stderr, "Can't open the file:  %s", Filename_1);
        exit(-1);
    }  

    int pipe1[2],pipe2[2];
    pipe_creation(pipe1);
    pipe_creation(pipe2);
    pid_t pid_1 = process_creation();
    pid_t pid_2 = 0;
    if (pid_1 == 0){
        // the 1st child
        close(pipe1[1]); // fd_pipe_1[1] for writing
        close(pipe2[0]); // fd_pipe_2[0] for reading
        close(pipe2[1]); // fd_pipe_2[1] for writing
        
        if(dup2(pipe1[0], STDIN_FILENO)==-1){
            
            perror("dup2 erorr ");
            exit(-1);
        }
        if(dup2(f1_output, STDOUT_FILENO)==-1){
            perror("dup2 erorr ");
            exit(-1);
        }
        if(dup2(f1_output, STDERR_FILENO)==-1){
            perror("dup2 erorr ");
            exit(-1);
        }
          
        if(execl("./child_1", "./child_1", NULL)==-1){
            perror("execl erorr ");
            exit(-1);
        }

        // close(pipe1[0]);
        // close(pipe2[1]);
        // close(f1_output);

    }else { 
        // parent
        write(STDOUT_FILENO, "\nEnter the second filename with file extension(.txt or .doc or .rtf): ", 71);
        char *Filename_2=NULL;

        if(inputing(&Filename_2 ,STDIN_FILENO, 0)<=0){
            perror("Trying to create 0-value string: ");
            exit(-1);
        }
        
        int f2_output=open(Filename_2, O_WRONLY | O_CREAT, 0777);
        free(Filename_2);
        if(f2_output==-1){
            fprintf(stderr, "Can't open the file:  %s", Filename_2);
            exit(-1);
        }     
    
        pid_t pid_2=process_creation();
        if(pid_2==0){
            //the 2nd child
            close(f1_output);
            close(pipe1[0]); // fd_pipe_1[0] for reading
            close(pipe1[1]); // fd_pipe_1[1] for writing
            close(pipe2[1]); // fd_pipe_2[1] for writing

            if(dup2(pipe2[0], STDIN_FILENO)==-1){
                
                perror("dup2 erorr ");
                exit(-1);
            }
            if(dup2(f2_output, STDOUT_FILENO)==-1){
                perror("dup2 erorr ");
                exit(-1);
            }
            if(dup2(f2_output, STDERR_FILENO)==-1){
                perror("dup2 erorr ");
                exit(-1);
            }
            
            if(execl("./child_2", "./child_2", NULL)==-1){
                perror(" execl erorr ");
                exit(-1);
            }

            // close(pipe2[0]);
            // close(f2_output);

        } 
        else
        { 
            // parent
            close(pipe1[0]);
            close(pipe2[0]); 

            write(STDOUT_FILENO, "Enter something you want: ", 27);
            while(true){
                char *s=NULL;
                int s_len=inputing(&s, STDIN_FILENO, 1);
                if(s_len==-1){
                    free(s);
                    break;
                }

                int prob_res=probability();

                if(prob_res==1){
                    if(write(pipe1[1], s, sizeof(char)*s_len)==-1){
                        perror("write error ");
                        exit(-1);
                    }
                    if (write(pipe2[1], "-", sizeof("-"))==-1){
                        perror("write error ");
                        exit(-1);
                    }

                } else{
                    if (write(pipe2[1], s, s_len*sizeof(char))==-1){
                        perror("write error ");
                        exit(-1);
                    }
                    if(write(pipe1[1], "-", sizeof("-"))==-1){
                        perror("write error ");
                        exit(-1);
                    }
                }
                free(s);
            }

        }
    }
    close(pipe1[1]);
    close(pipe2[1]);
    close(f1_output);
    close(f2_output);
    kill(pid_1, SIGTERM);
    kill(pid_2, SIGTERM);
    write(STDOUT_FILENO, "\nProgramm was ended successfully!\n", 35);
}