#ifndef __scheduler__
#define __scheduler__
#include "../include/support.h"
#include "../include/cdata.h"

#define FPRIO_PRIORITY_HIGH 0
#define FPRIO_PRIORITY_MEDIUM 1
#define FPRIO_PRIORITY_LOW 2

ucontext_t scheduler;
char ss_scheduler[1024*32];

// Fila de prioridades
typedef struct FPrio {
    PFILA2 high;
    PFILA2 medium;
    PFILA2 low;
} FILAPRIO;

/*
Estrutura para a fila de JOINS, para a função CJOINS
*/
typedef struct joins{
    int tid_blockedThread; //pid da thread que esta bloqueada
    int tid_blockingThread; //pid da thread que está bloqueando a thread bloqueada
} joint;

typedef struct FPrio * PFILAPRIO;

ucontext_t mainThreadContext; // Utilizado para ir e voltar para main thread
TCB_t * mainThreadTCB;


/**
 *  Inicia Thread com tid = 0
 * */
int initMainThread();

/**
 *  Escolhe e roda Thread na fila de aptos
 * */
int chooseAndRunReadyThread();


/**
 * Checa se a fila esta vazia
 * */
int isEmptyFila(PFILA2 fila);

/**
 * Coloca a thread na fila de executando e executa
 * */
int runThread(TCB_t * content);

/**
 *  Cria a fila de aptos
 * */
int createFilaPrioridades();

/**
 * Cria uma thread a partir do contexto e prioridade
 * */
TCB_t *createThread(ucontext_t context, int priority);

/**
 * Procura uma thread a partir de um tid, retorna a thread
 * */
TCB_t *findThread(int tid);

/**
 * Funcao auxiliar a findthread, procura pelo tid na fila especificada
 * */
void searchForThreadInside(PFILA2 fila, TCB_t ** content, int tid);

/**
 *  Insere contexto na fila de aptos e cria um tid
 * */
int insertFilaPrioridades(TCB_t * content);

/**
*  Pega a primeira thread da fila de prioridades a partir da prioridade escolhida
* */
TCB_t *getAtFilaPrioridades(int priority);


/**
* Pega a primeira thread de uma fila e retira ela da fila
* */
TCB_t *getAndDeleteFirstFila(PFILA2 fila);

/**
* Cria uma fila comum (executando, bloqueado, terminado)
*/
int initStdFila(PFILA2 * fila);

int priorityYield();

int prepareYield();

int yield();

int setRunningThreadPriority(int priority);

/*
* Remove a thread da fila running e roda novamente a chose
*/
void finishThread();

/*
Cjoin
*/
int blockedForThread(int tid);

/*
* Tira a thread que está rodando (fila running) e coloca na fila de bloqueadas
*/
TCB_t * blockThread();

/*
* Tira a thread da fila de bloqueados e coloca na de aptos denovo
*/
void unblockThread(int tid_blockedThread);

/*
* Procura uma thread em uma fila normal
*/
int FindThreadByNormalFila(PFILA2 fila, int tid);

/*
* Varre a fila de espera procurando por alguma thread que esteja esperando a thread solicitada(parametro tid).
* Retorna ERROR caso já tenha alguma thread esperando a thread solicitada
* (Uma thread não pode ser aguardado por mais de duas threads).
*/
int isBlocker(int tid);
/*
* Veririfica se a thread esta em aptos ou blockeados
*/
int findThreadAptosBlockeds(int tid);

#endif
