#include <stdio.h>
#include "function.h"


float Derivative(float a, float dx){ // на вход получаем угол в градусах, а приращение в радианах
    float answ=(cosf((a*PI)/180+dx)- cosf((a*PI)/180))/dx;
    return answ;
}


void Sort(int *array, int size){
    // printf("obobus");
    // for(int i=0; i<size; ++i){
    //     printf("%d ", array[i]);
    // }
    for(int i=0; i< size;++i){
        int len1=size-i;
        bool change_stat=false;
        for(int j=0; j<len1-1;++j){
            if(array[j]>array[j+1]){
                int tmp=array[j+1];
                array[j+1]=array[j];
                array[j]=tmp;
                change_stat=true;
            }
        }
        if(change_stat==false){
            break;
        }
    }
}

