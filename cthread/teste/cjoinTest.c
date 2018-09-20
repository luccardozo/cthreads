#include <stdio.h>
#include <stdlib.h>
#include "../include/scheduler.h"
#include "../include/cthread.h"
#include "../include/thread.h"
//gcc -o cjoinTest cjoinTest.c ../src/thread.c ../src/scheduler.c ../src/lib.c ../bin/support.o -Wall -ggdb && ./cjoinTest

void function1();
void function2();
void function3();
void function4();

int thread1;
int thread2;
int thread3;
int thread4;

int main(){

    printf("Começando o programa\n\n");
    thread1 = ccreate((void*)&function1, 0, FPRIO_PRIORITY_HIGH);

    printf("\nVoltei para a main\n");
    printf("Tid thread 1: %d \n", thread1);
    int aux = 0;
    printf("Variavel auxiliar: %d\n", aux);

    return 0;
}

void function1(){
    
    printf("Sou a função 1\n");
    thread2 = ccreate((void*)&function2, 0, FPRIO_PRIORITY_HIGH);
    thread3 = ccreate((void*)&function3, 0, FPRIO_PRIORITY_MEDIUM);
    thread4 = ccreate((void*)&function4, 0, FPRIO_PRIORITY_HIGH);

    printf("Finalizada a função 1\n");   
}
void function2(){
    printf("\nSou a função 2, e vou terminar quando a medium(3) terminar:\n");
    cjoin(thread3);
    printf("Agora que a thread3 terminou, vou finalizar!\n");
}
void function3(){
    printf("\nSou a função3. Vou finalizar e liberar a 2\n");
}
void function4(){
    printf("\nSou a função 4, e vou terminar quando a medium(3) terminar:\n");
    int wait = cjoin(thread3);
    if(wait != 0){
        printf("OPS. Eu nao posso esperar pela thread3, já tem algum esperando por ela.\n");
    }
}