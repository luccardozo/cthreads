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

ucontext_t mainThreadContext; // Utilizado para ir e voltar para main thread



int initMainThread() {
    TCB_t * content = malloc(sizeof(TCB_t));

    initStdFila(&blocked);
    initStdFila(&running);
    initStdFila(&finished);

    createFilaPrioridades();

    getcontext(&mainThreadContext);

    content = createThread(mainThreadContext, FPRIO_PRIORITY_LOW);
    
    if (content != NULL) {
        insertFilaPrioridades(content);
        AppendFila2(running, content);
        chooseAndRunReadyThread();
        return 0;
    } else {
        return -1;
    }

}

int chooseAndRunReadyThread(){
    TCB_t * content = malloc(sizeof(TCB_t));
    TCB_t * runningContent = malloc(sizeof(TCB_t));

    if(!isEmptyFila(running)) {
        printf("Não ta vazia! Retorno: %d\n\n", isEmptyFila(running));
        FirstFila2(running);
        runningContent = (TCB_t*)GetAtIteratorFila2(running);

        if (!isEmptyFila(filaPrioridades->high) && runningContent->prio != FPRIO_PRIORITY_HIGH){
            content = getAtFilaPrioridades(FPRIO_PRIORITY_HIGH);
            runThread(content);
            return 0;
        } else if (!isEmptyFila(filaPrioridades->medium) && (runningContent->prio != FPRIO_PRIORITY_MEDIUM || runningContent->prio != FPRIO_PRIORITY_HIGH)) {
            content = getAtFilaPrioridades(FPRIO_PRIORITY_MEDIUM);
            runThread(content);
            return 0;
        }
        return 0;
    } else {
        printf("Ta vazia!\n\n");
        if (!isEmptyFila(filaPrioridades->high)){
            content = getAtFilaPrioridades(FPRIO_PRIORITY_HIGH);
            runThread(content);
            return 0;
        } else if (!isEmptyFila(filaPrioridades->medium)) {
            content = getAtFilaPrioridades(FPRIO_PRIORITY_MEDIUM);
            runThread(content);
            return 0;
        } else if (!isEmptyFila(filaPrioridades->low)) {
            content = getAtFilaPrioridades(FPRIO_PRIORITY_LOW);
            runThread(content);
            return 0;
        } else {
            return -1;
        }
    }
}

int isEmptyFila(PFILA2 fila) {
    void * content;
    FirstFila2(fila);
    content = GetAtIteratorFila2(fila);

    if (content == NULL) {
        return 1;
    } else {
        return 0;
    }
}

// TODO: Rodar a thread, alem de colocar na fila de running
int runThread(TCB_t * content) {
    int state = PROCST_EXEC;
    content->state = state;
    AppendFila2(running, content);
    setcontext(&(content->context));//roda a thread
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

    TCB_t * content = malloc(sizeof(TCB_t));
    *content = (TCB_t) {.tid = tid, .state = state, .prio = priority, .context = context};
    tid += 1;
    return content;

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
    TCB_t * content = malloc(sizeof(TCB_t));
    content->tid = -1; // tid impossivel, para fazer comparacoes

    searchForThreadInside(running, &content, tid);
    searchForThreadInside(filaPrioridades->high, &content, tid);
    searchForThreadInside(filaPrioridades->medium, &content, tid);
    searchForThreadInside(filaPrioridades->low, &content, tid);
    searchForThreadInside(blocked, &content, tid);
    searchForThreadInside(finished, &content, tid);

    if(content->tid == tid){
        return content;
    } else {
        return NULL;
    }
    
}

void searchForThreadInside(PFILA2 fila, TCB_t ** content, int tid) {
    TCB_t * searchContent = malloc(sizeof(TCB_t));

    FirstFila2(fila);
    while (GetAtIteratorFila2(fila) != NULL && (*content)->tid != tid) {
        searchContent = (TCB_t*)GetAtIteratorFila2(fila);
        NextFila2(fila);

        if(searchContent->tid == tid){
            *content = searchContent;
        }
    }

    return;
}


int insertFilaPrioridades(TCB_t * content) {
    int state = PROCST_APTO;

    content->state = state;
    
    switch (content->prio) {
        case FPRIO_PRIORITY_HIGH:
            AppendFila2(filaPrioridades->high, content);
            break;
        case FPRIO_PRIORITY_MEDIUM:
            AppendFila2(filaPrioridades->medium, content);
            break;
        case FPRIO_PRIORITY_LOW:
            AppendFila2(filaPrioridades->low, content);
            break;
        default:
            return -1;

    }

    return 0;
}

TCB_t *getAtFilaPrioridades(int priority) {
    TCB_t * content = malloc(sizeof(TCB_t));
    switch (priority) {
        case FPRIO_PRIORITY_HIGH:
            content = getAndDeleteFirstFila(filaPrioridades->high);
            break;
        case FPRIO_PRIORITY_MEDIUM:
            content = getAndDeleteFirstFila(filaPrioridades->medium);
            break;
        case FPRIO_PRIORITY_LOW:
            content = getAndDeleteFirstFila(filaPrioridades->low);
            break;
        default:
            return NULL;
    }
  
    return content;
}

TCB_t *getAndDeleteFirstFila(PFILA2 fila) {
    TCB_t * content = malloc(sizeof(TCB_t));
    FirstFila2(fila);
    content = (TCB_t*)GetAtIteratorFila2(fila);
    DeleteAtIteratorFila2(fila);
    return content;
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

void finishThread(){
    TCB_t * thread = (TCB_t*)GetAtIteratorFila2(running); //Pega a thread da fila de execução
    DeleteAtIteratorFila2(running); //Remove a thread da fila de execução
    AppendFila2(finished, thread); //Coloca a thread na fila de terminados
    chooseAndRunReadyThread(); //Executa uma nova thread.
}