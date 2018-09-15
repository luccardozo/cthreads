#include <stdio.h>
#include <stdlib.h>
#include "../include/scheduler.h"
#include "../include/cthread.h"
#include "../include/thread.h"
//gcc -o threadTest threadTest.c ../src/thread.c ../src/scheduler.c ../src/lib.c ../bin/support.o -Wall -ggdb && ./sch threadTest

int function1();
int function2();
//int function3();

int main(){
    int thread1 = ccreate((void*)&function1, 0, 2);
    int thread2 = ccreate((void*)&function2, 0, 0);

    printf("Tid thread 1: %d \n", thread1);
    printf("Tid thread 2: %d \n", thread2);

    return 0;
}

int function1(){
    printf("Sou a função 1\n");
    return 0;
}

int function2(){
    printf("Sou a função 1\n");
    return 0;
}
