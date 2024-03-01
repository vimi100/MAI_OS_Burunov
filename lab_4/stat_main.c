#include <stdio.h>
#include "function.h"
#include <stdbool.h>

int main(){
    int com_numb;
    while(true){
        printf("\nChoose comand. Press: \n- 1 for cos derivative\n- 2 for sorting array\n");
        if(scanf("%d", &com_numb)==EOF){
            printf("\nProgram has been ended. Bye!\n");
            break;
        }
        switch(com_numb){
            case 1:
            {
                float angle, dx;
                printf("Input angle in degrees ");
                scanf("%f", &angle);
                printf("Input Dx in radians ");
                scanf("%f", &dx);
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
}