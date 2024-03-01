#include <stdio.h>
#include "function.h"

float Derivative(float a, float dx){ // на вход получаем угол в градусах, а приращение в радианах
    float answ=(cosf((a*PI)/180+dx)- cosf((a*PI)/180-dx))/(2*dx);
    return answ;
}

int minim(int a, int b, int c){
    int mn;
    if (a<b){
        mn= a;
    }else {
        mn =b;
    }
    if (c<mn){
        mn=c;
    }
    return mn;
}

int maxim(int a, int b, int c){
    int mx;
    if(a>b){
        mx=a;
    } else{
        mx=b;
    }
    if(mx<c){
        mx=c;
    }
    return mx;
}

void swap(int *array, int i1, int i2){
    if(i1 == i2){
        return;
    } 
    int tmp = array[i1];
    array[i1]=array[i2];
    array[i2]=tmp;

}

int find_pivot(int *array, int size){
    int a,b,c, pivot;
    a=array[0];
    b=array[size-1];
    c=array[size/2];
    pivot=a+b+c-maxim(a,b,c)-minim(a,b,c);
    // printf("Pivot = %d\n", pivot);
    return pivot;
}

// void partition(int *array, int *left, int *mid, int* right, int pivot){

// }

void partition(int *array, int *kf_i, int *kf_k, int size){
    int pivot=find_pivot(array, size);
    int i=0;
    int j=0;
    int k=0;
    while(j<size){
        if(array[j]<pivot){
            swap(array, i, j);
            if(i!=k){
                swap(array, k, j);
            }
            i++;
            k++;
        } else if(array[j]==pivot){
            swap(array, k, j);
            k++;
        }
        j++;
    }
    *kf_k=k;
    *kf_i=i;
}


void quicksort(int *array, int size){
    if(size <2){
        return;
    } else if(size==2){
        if(array[0]>array[1]){
            swap(array, 0, 1);
        }
        return;
    }
    int i,k;
    partition(array, &i, &k, size);
    quicksort(array, i);
    quicksort(&array[k], size-k);

}


void Sort(int *array, int size){
    quicksort(array, size);
}
