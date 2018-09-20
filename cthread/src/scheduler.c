#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <cdata.h>
#include <ucontext.h>
//#include <thread.h>
#include "../include/support.h"
#include "../include/scheduler.h"
#include "../include/cdata.h"

PFILA2 running, blocked, finished, filaJoints; // Filas comuns para executando, bloqueados, terminados

PFILAPRIO filaPrioridades; // Fila de prioridades para aptos

int tid = 0; // ID de threads


int initMainThread() {
    mainThreadTCB = malloc(sizeof(TCB_t));

    initStdFila(&blocked);
    initStdFila(&running);
    initStdFila(&finished);
    initStdFila(&filaJoints);

    createFilaPrioridades();

    getcontext(&mainThreadContext);

    mainThreadTCB = createThread(mainThreadContext, FPRIO_PRIORITY_LOW);
    
    if (mainThreadTCB != NULL) {

        mainThreadTCB->state = PROCST_EXEC;
        AppendFila2(running, mainThreadTCB);

        return 0;
    } else {
        return -1;
    }

}

int chooseAndRunReadyThread(){
    TCB_t * thread = malloc(sizeof(TCB_t));

    if (!isEmptyFila(filaPrioridades->high)){
        thread = getAtFilaPrioridades(FPRIO_PRIORITY_HIGH);
        runThread(thread);
        return 0;
    } else if (!isEmptyFila(filaPrioridades->medium)) {
        thread = getAtFilaPrioridades(FPRIO_PRIORITY_MEDIUM);
        runThread(thread);
        return 0;
    } else if (!isEmptyFila(filaPrioridades->low)) {
        thread = getAtFilaPrioridades(FPRIO_PRIORITY_LOW);
        runThread(thread);
        return 0;
    } else {
        return -1;
    }
}

int isEmptyFila(PFILA2 fila) {
    void * thread;
    FirstFila2(fila);
    thread = GetAtIteratorFila2(fila);

    if (thread == NULL) {
        return 1;
    } else {
        return 0;
    }
}

int runThread(TCB_t * thread) {
    int state = PROCST_EXEC;
    thread->state = state;
    AppendFila2(running, thread);
    setcontext(&(thread->context));//roda a thread
    return 0;
}

int createFilaPrioridades() {
    filaPrioridades = malloc(sizeof(PFILAPRIO));

    if (filaPrioridades->high == NULL) {
        filaPrioridades->high = malloc(sizeof(PFILA2));
        CreateFila2(filaPrioridades->high);
    }
    if (filaPrioridades->medium == NULL) {
        filaPrioridades->medium = malloc(sizeof(PFILA2));
        CreateFila2(filaPrioridades->medium);
    }
    if (filaPrioridades->low == NULL) {
        filaPrioridades->low = malloc(sizeof(PFILA2));
        CreateFila2(filaPrioridades->low);
    }
    return 0;
}

TCB_t *createThread(ucontext_t context, int priority) {
    int state = PROCST_CRIACAO;

    TCB_t * thread = malloc(sizeof(TCB_t));
    *thread = (TCB_t) {.tid = tid, .state = state, .prio = priority, .context = context};
    tid += 1;
    return thread;

}

TCB_t *findThread(int tid) {
    TCB_t * thread = malloc(sizeof(TCB_t));
    thread->tid = -1; // tid impossivel, para fazer comparacoes

    searchForThreadInside(running, &thread, tid);
    searchForThreadInside(filaPrioridades->high, &thread, tid);
    searchForThreadInside(filaPrioridades->medium, &thread, tid);
    searchForThreadInside(filaPrioridades->low, &thread, tid);
    searchForThreadInside(blocked, &thread, tid);
    searchForThreadInside(finished, &thread, tid);

    if(thread->tid == tid){
        return thread;
    } else {
        return NULL;
    }
    
}

void searchForThreadInside(PFILA2 fila, TCB_t ** thread, int tid) {
    TCB_t * searchthread = malloc(sizeof(TCB_t));

    FirstFila2(fila);
    while (GetAtIteratorFila2(fila) != NULL && (*thread)->tid != tid) {
        searchthread = (TCB_t*)GetAtIteratorFila2(fila);
        NextFila2(fila);

        if(searchthread->tid == tid){
            *thread = searchthread;
        }
    }

    return;
}


int insertFilaPrioridades(TCB_t * thread) {
    int state = PROCST_APTO;

    thread->state = state;
    
    switch (thread->prio) {
        case FPRIO_PRIORITY_HIGH:
            AppendFila2(filaPrioridades->high, thread);
            break;
        case FPRIO_PRIORITY_MEDIUM:
            AppendFila2(filaPrioridades->medium, thread);
            break;
        case FPRIO_PRIORITY_LOW:
            AppendFila2(filaPrioridades->low, thread);
            break;
        default:
            return -1;

    }

    return 0;
}

TCB_t *getAtFilaPrioridades(int priority) {
    TCB_t * thread = malloc(sizeof(TCB_t));
    switch (priority) {
        case FPRIO_PRIORITY_HIGH:
            thread = getAndDeleteFirstFila(filaPrioridades->high);
            break;
        case FPRIO_PRIORITY_MEDIUM:
            thread = getAndDeleteFirstFila(filaPrioridades->medium);
            break;
        case FPRIO_PRIORITY_LOW:
            thread = getAndDeleteFirstFila(filaPrioridades->low);
            break;
        default:
            return NULL;
    }
  
    return thread;
}

TCB_t *getAndDeleteFirstFila(PFILA2 fila) {
    TCB_t * thread = malloc(sizeof(TCB_t));
    FirstFila2(fila);
    thread = (TCB_t*)GetAtIteratorFila2(fila);
    DeleteAtIteratorFila2(fila);
    return thread;
}

int initStdFila(PFILA2 * fila) {
    if (*fila == NULL) {
        *fila = malloc(sizeof(PFILA2));
        CreateFila2(*fila);
        return 0;
    } else {
        return -1;
    }
    
}

int priorityYield() {
    TCB_t * thread =  malloc(sizeof(TCB_t));
    int hasYielded = 0;
    FirstFila2(running);
    thread = (TCB_t*)GetAtIteratorFila2(running);

    // Salva o contexto da thread rodando, para quando retornar, continuar de onde parou
    getcontext(&(thread->context));

    if (!hasYielded) {
        hasYielded = 1;
        if (thread->prio == FPRIO_PRIORITY_HIGH) {
            return 0;
        } else if (thread->prio == FPRIO_PRIORITY_MEDIUM && !isEmptyFila(filaPrioridades->high)) {
            return yield();
        } else if (thread->prio == FPRIO_PRIORITY_LOW && (!isEmptyFila(filaPrioridades->high) ||  !isEmptyFila(filaPrioridades->medium))) {
            return yield();
        } else {
            return 0;
        }
    }
    return 0;
    
}

int prepareYield() {
    TCB_t * thread =  malloc(sizeof(TCB_t));
    int hasYielded = 0;

    FirstFila2(running);
    thread = (TCB_t*)GetAtIteratorFila2(running);

    getcontext(&(thread->context));

    if (!hasYielded) {
        hasYielded = 1;
        return yield();
    }
    return 0;
}

int yield() {
    TCB_t * thread = getAndDeleteFirstFila(running); //Pega a thread da fila de execução
    if (thread == NULL) {
        return -1;
    }
    insertFilaPrioridades(thread);
    chooseAndRunReadyThread();
    return 0;
}

void finishThread(){
    TCB_t * thread = getAndDeleteFirstFila(running); //Pega a thread da fila de execução
    thread->state = PROCST_TERMINO;

/*Verifica se a thread que está finalizando não é era uma thread bloqueante e já faz o tratamento*/
    joint * join;//variavel para varer a fila
    FirstFila2(filaJoints); //Posiciona-se no inicio da fila
    do {  //Varre a fila até o fim procurando pelo tid
        join = (joint*)GetAtIteratorFila2(filaJoints);
        if(join == NULL)
            break;
        if(join->pid_threadBlocking == thread->tid){ //se a thread bloqueante é igual a que saiu
            SearchThreadByTidFila2(blocked, join->pid_thread);
            TCB_t * blockedThread = (TCB_t*)GetAtIteratorFila2(blocked);
            DeleteAtIteratorFila2(blocked);
            insertFilaPrioridades(blockedThread);//bota na fila de aptos denovo
        }
    }while(NextFila2(filaJoints) == 0);
/*Termino da verificação e tratamento de bloqueante^^^^ */

    AppendFila2(finished, thread); //Coloca a thread na fila de terminados
    chooseAndRunReadyThread(); //Executa uma nova thread.
}

int setRunningThreadPriority(int priority) {
    TCB_t * thread = getAndDeleteFirstFila(running); //Pega a thread da fila de execução
    if (thread == NULL) {
        return -1;
    } else {
        thread->prio = priority;
        AppendFila2(running, thread);
        return 0;
    }
}
int blockedForThread(int tid){
    joint *join;//variavel para percorrer a fila de joins
	//findWaiting(join, tid);
    FirstFila2(filaJoints);
    do {  //Varre a fila de espera procurando por alguma thread que esteja esperando a thread solicitada
        join = (joint*)GetAtIteratorFila2(filaJoints);
        if(join == NULL) 
            break;
        if(join->pid_threadBlocking == tid){ //Não podem haver duas threads esperando a mesma thread
            return -1;
            break;
        }
    }while(NextFila2(filaJoints) == 0);
	TCB_t * blockedThread = blockThread();//bloqueia a thread

	/*Alocamento da JOIN*/
	join = malloc(sizeof(joint));
	join->pid_thread = blockedThread->tid;
	join->pid_threadBlocking = tid;
    //printf("Thread bloqueada tid = %d\nThread bloqueando = %d\n", join->pid_thread, join->pid_threadBlocking);
	AppendFila2(filaJoints, join); //Coloca na fila de espera

    /*Variavel para proteger contexto*/
    int isCommingBack = 1;

	getcontext(&(blockedThread->context));//ponto de retorno qnd desbloqueada

    if(isCommingBack == 1){
        isCommingBack = 0;
        chooseAndRunReadyThread(); //Sorteia a proxima thread a ser executada
    }
    return 0;
}

/*
Tira a thread que está rodando (fila running) e coloca na fila de bloqueadas
*/
TCB_t * blockThread(){
    TCB_t * thread = getAndDeleteFirstFila(running);
    thread->state = PROCST_BLOQ;
    AppendFila2(blocked, thread); //Coloca a thread na fila de bloqueados
    return thread; //Retorna a thread que foi bloqueada
}

int unblockThread(TCB_t * thread){
  RemoveThreadFila2(blocked, thread->tid); //Remove a thread da fila de bloqueados
  insertFilaPrioridades(thread); //Adiciona a thread à fila de aptos
  return 0;
}
/*Procura uma thread em uma fila normal*/
int SearchThreadByTidFila2(PFILA2 fila, int tid){
  TCB_t * itThread = NULL;
  if(FirstFila2(fila) != 0) return -1; //Posiciona-se no inicio da fila
  do {  //Varre a fila até o fim procurando pelo tid
    itThread = (TCB_t*)GetAtIteratorFila2(fila);
    if(itThread == NULL) break;
    if(itThread->tid == tid){ //Se achar o tid, termina a busca
      return 0;
      break;
    }
  }while(NextFila2(fila) == 0);
  return -1;
}

int RemoveThreadFila2(PFILA2 fila, int tid){
  if(SearchThreadByTidFila2(fila, tid) == -1) return -1;
  if(DeleteAtIteratorFila2(fila) == -1) return -1;
  return 0;
}


















/*
Varre a fila de espera procurando por alguma thread que esteja esperando a thread solicitada.
Retorna ERROR caso já tenha alguma thread esperando a thread solicitada
(Uma thread não pode ser aguardado por mais de duas threads).

int findWaiting(joint *join, int tid){
	FirstFila2(filaJoints);//pega o começo da fila
	do{
		join = (joint*)GetAtIteratorFila2(filaJoints);
		if(join == NULL)//fim da fila, e não encontrou
			break;
		if(join->pid_threadBlocking == tid){//só uma thread pode esperar
			return -1;
			break;		
		}		
	}while(NextFila2(filaJoints) == 0);
}*/