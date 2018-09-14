#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/scheduler.h"
#include "../include/cdata.h"

PFILA2 running, blocked, finished, creating; // Filas comuns para executando, bloqueados, terminados e criando

PFILAPRIO filaPrioridades; // Fila de prioridades para aptos

unsigned int tid = 0; // ID de threads

ucontext_t mainThreadContext; // Utilizado para ir e voltar para main thread

int initMainThread() {
    TCB_t * content = malloc(sizeof(TCB_t));

    initStdFila(&creating);
    initStdFila(&running);
    createFilaPrioridades();

    getcontext(&mainThreadContext);

    createThread(mainThreadContext);

    content = getAndDeleteLastFila(creating);

    if (content != NULL) {
        insertFilaPrioridades(content, FPRIO_PRIORITY_LOW);

        return 0;
    } else {
        return -1;
    }


}


/*
int chooseThread(){
    if(){

    }
}

int runThread(TCB_t * content) {
    int state = STATE_RUNNING;


}
*/

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

int createThread(ucontext_t context) {
    int state = STATE_CREATION;
    int priority = 0;

    TCB_t * content = malloc(sizeof(TCB_t));
    *content = (TCB_t) {.tid = tid, .state = state, .prio = priority, .context = context};
    AppendFila2(creating, content);
    tid += 1;
    return 0;


}


int insertFilaPrioridades(TCB_t * content, int priority) {
    int state = STATE_READY;

    content->state = state;
    
    switch (priority) {
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
            content = getAndDeleteLastFila(filaPrioridades->high);
            break;
        case FPRIO_PRIORITY_MEDIUM:
            content = getAndDeleteLastFila(filaPrioridades->medium);
            break;
        case FPRIO_PRIORITY_LOW:
            content = getAndDeleteLastFila(filaPrioridades->low);
            break;
        default:
            return NULL;
    }
  
    return content;
}

TCB_t *getAndDeleteLastFila(PFILA2 fila) {
    TCB_t * content = malloc(sizeof(TCB_t));
    LastFila2(fila);
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

