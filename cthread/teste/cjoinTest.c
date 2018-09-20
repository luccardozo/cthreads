#include <stdio.h>
#include <stdlib.h>
#include "../include/scheduler.h"
#include "../include/cthread.h"
#include "../include/thread.h"
//gcc -o cjoinTest cjoinTest.c ../src/thread.c ../src/scheduler.c ../src/lib.c ../bin/support.o -Wall -ggdb && ./cjoinTest

void function1();
void function2();
void function3();

int thread1;
int thread2;
int thread3;
int main(){

    printf("Começando o programa\n\n");
    thread1 = ccreate((void*)&function1, 0, FPRIO_PRIORITY_HIGH);
  
    printf("\nEstou na main\n");
    printf("Tid thread 1: %d \n", thread1);

    int aux = 0;
    printf("Variavel de saida: %d\n", aux);

    return 0;
}

void function1(){
    
    printf("Sou a função 1\n");

    thread2 = ccreate((void*)&function2, 0, FPRIO_PRIORITY_HIGH);
    thread3 = ccreate((void*)&function3, 0, FPRIO_PRIORITY_MEDIUM);

    printf("Sou o final da função 1\n\n");    
}
void function2(){
    printf("Sou a função 2, e só vou terminar quando a media(3) termianr\n");
    cjoin(thread3);
    printf("Agora que a função 3 terminou eu estou livre leve e solta e vou finalizar\n");
}
void function3(){
    printf("Sou a função 3\n");
}