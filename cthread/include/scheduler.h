#ifndef __scheduler__
#define __scheduler__
#include "../include/support.h"
#include "../include/cdata.h"

#define STATE_CREATION 0
#define STATE_READY 1
#define STATE_RUNNING 2
#define STATE_BLOCKED 3
#define STATE_FINISHED 4

#define FPRIO_PRIORITY_HIGH 0
#define FPRIO_PRIORITY_MEDIUM 1
#define FPRIO_PRIORITY_LOW 2

// Fila de prioridades
typedef struct FPrio {
    PFILA2 high;
    PFILA2 medium;
    PFILA2 low;
} FILAPRIO;

typedef struct FPrio * PFILAPRIO;


int initMainThread();

int chooseReadyThread();

int isEmptyFila(PFILA2 fila);

int runThread(TCB_t * content);

/**
 *  Cria a fila de aptos
 * */
int createFilaPrioridades();

TCB_t *createThread(ucontext_t context, int priority);

TCB_t *findThread(int tid);

void searchForThreadInside(PFILA2 fila, TCB_t ** content, int tid);

/**
 *  Insere contexto na fila de aptos e cria um tid
 * */
int insertFilaPrioridades(TCB_t * content);

TCB_t *getAtFilaPrioridades(int priority);

TCB_t *getAndDeleteFirstFila(PFILA2 fila);

/**
 * Cria uma fila comum (executando, bloqueado, terminado)
 * */
int initStdFila(PFILA2 * fila);

#endif
