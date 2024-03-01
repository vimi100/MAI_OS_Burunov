#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>

int main(){
    void *hld=dlopen("./libfirst_realization.so", RTLD_LAZY);
    if(hld==NULL){
        fputs(dlerror(), stderr);
        exit(-1);
    }
    int reliz_numb=1;
    int com_numb;
    char *error;
    while(true){
        printf("\nChoose comand. Press: \n- 0 for changing realization \n- 1 for cos derivative\n- 2 for sorting array\n");
        if(scanf("%d", &com_numb)==EOF){
            printf("\nProgram has been ended. Bye!\n");
            break;
        }
        switch(com_numb){
            case 0:
            {   
                if(dlclose(hld)!=0){
                    fputs(dlerror(), stderr);
                    exit(-1);
                }
                int last_reliz_numb=reliz_numb;
                if(reliz_numb==1){
                    hld=dlopen("./libsecond_realization.so", RTLD_LAZY);
                    reliz_numb=2;
                } else{
                    hld=dlopen("./libfirst_realization.so", RTLD_LAZY);
                    reliz_numb=1;
                }
                if(hld==NULL){
                    fputs(dlerror(), stderr);
                    exit(-1);
                }
                printf("Realization was changed from realization%d to realization%d.\n", last_reliz_numb ,reliz_numb);

                break;
            }
            case 1:
            {
                float angle, dx;
                printf("Input angle in degrees ");
                scanf("%f", &angle);
                printf("Input Dx in radians ");
                scanf("%f", &dx);

                float (*Derivative)(float, float);
                *(float **) (&Derivative)=dlsym(hld, "Derivative");
                if ((error = dlerror()) != NULL)  {
                    fprintf(stderr, "%s\n", error);
                    exit(1);
                }

                float answ=Derivative(angle, dx);
                printf("Answ: %f\n", answ);
                break;
            }
            case 2:
            {
                int a_size;
                printf("Input array size ");
                scanf("%d", &a_size);
                int array[a_size];
                printf("\nInput elements: ");
                for(int i=0; i<a_size;++i){
                    int tmp;
                    scanf("%d", &tmp);
                    array[i]=tmp;
                    // printf("hh%d ", array[i]);
                    // printf("hui%d ", i);
                }

                void (*Sort)(int *, int);
                *(void **)(&Sort) = dlsym(hld, "Sort");
                if ((error = dlerror()) != NULL)  {
                    fprintf(stderr, "%s\n", error);
                    exit(1);
                }
                

                Sort(array, a_size);
                printf("Array was sorted: ");
                for(int i=0; i<a_size;++i){
                    
                    printf("% d", array[i]);
                }
                break;
            }
            default:
                printf("Uncorrect inputting. Try again\n");
                break;

        }
    }
    if (dlclose(hld) != 0) {
        perror("dlclose");
        exit(1);
    }
}