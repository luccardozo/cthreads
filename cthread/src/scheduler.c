#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <cdata.h>
#include <ucontext.h>
//#include <thread.h>
#include "../include/support.h"
#include "../include/scheduler.h"
#include "../include/cdata.h"

PFILA2 running, blocked, finished, joints; // Filas comuns para executando, bloqueados, terminados

PFILAPRIO filaPrioridades; // Fila de prioridades para aptos

int tid = 0; // ID de threads


int initMainThread() {
    mainThreadTCB = malloc(sizeof(TCB_t));

    initStdFila(&blocked);
    initStdFila(&running);
    initStdFila(&finished);

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
void createThreadTest(TCB_t * thread, ucontext_t * context, int priority){
    int state = PROCST_CRIACAO;
    thread->state = state;
    thread-> tid = tid;
    thread->prio = priority;
    thread->context = *context;
    tid +=1;
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