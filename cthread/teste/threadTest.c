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
void function5();
void function6();

int main(){

    printf("Começando o programa\n\n");
    int thread1 = ccreate((void*)&function1, 0, FPRIO_PRIORITY_MEDIUM);
    int thread2 = ccreate((void*)&function2, 0, FPRIO_PRIORITY_HIGH);
    int thread3 = ccreate((void*)&function3, 0, FPRIO_PRIORITY_MEDIUM);
    int thread4 = ccreate((void*)&function4, 0, FPRIO_PRIORITY_HIGH);
    

    printf("Tid thread 1: %d \n", thread1);
    printf("Tid thread 2: %d \n", thread2);
    printf("Tid thread 3: %d \n", thread3);
    printf("Tid thread 4: %d \n", thread4);


    int aux = 0;
    printf("Variavel auxiliar: %d\n", aux);

    return 0;
}

void function1(){
    
    printf("Sou a função 1\n");
    int thread5 = ccreate((void*)&function5, 0, FPRIO_PRIORITY_MEDIUM);
    printf("Tid thread 5: %d \n", thread5);

    printf("Fazendo Yield antes de imprimir, funcao 5 deve rodar\n");

    cyield();

    printf("Apos Yield, de volta a funcao 1\n");
    
}
void function5(){
    printf("Sou a funcao 5, dentro da 1\n");
    printf("Setar prioridade para baixa, funcao 1 deve voltar\n");
    csetprio(0,FPRIO_PRIORITY_LOW);
    printf("De volta a funcao 5, agora com prioridade baixa\n");

    int thread6 = ccreate((void*)&function6, 0, FPRIO_PRIORITY_LOW);
    printf("Tid thread 6: %d \n", thread6);
}

void function6(){
    printf("Sou a funcao 6, dentro da 5, prioridade baixa\n");

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
