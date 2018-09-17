#include <stdio.h>
#include <stdlib.h>
#include "../include/scheduler.h"
#include "../include/cthread.h"
#include "../include/thread.h"
//gcc -o threadTest threadTest.c ../src/thread.c ../src/scheduler.c ../src/lib.c ../bin/support.o -Wall -ggdb && ./threadTest

void function1();
void function2();
void function3();
void function4();

int main(){

    printf("Començando o programa\n\n");
    int thread1 = ccreate((void*)&function1, 0, 1);
    int thread2 = ccreate((void*)&function2, 0, 0);
    int thread3 = ccreate((void*)&function3, 0, 1);
    int thread4 = ccreate((void*)&function4, 0, 0);
    

    printf("Tid thread 1: %d \n", thread1);
    printf("Tid thread 2: %d \n", thread2);
    printf("Tid thread 3: %d \n", thread3);
    printf("Tid thread 4: %d \n", thread4);


    int aux = chooseAndRunReadyThread();

    printf("Variavel auxiliar: %d", aux);

    return 0;
}

void function1(){
    printf("Sou a função 1\n");
}
void function2(){
    printf("Sou a função 2\n");
}
void function3(){
    printf("Sou a função 3\n");
}
void function4(){
    printf("Sou a função 4\n");
}