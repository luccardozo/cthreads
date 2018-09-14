#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/scheduler.h"
#include "../include/cdata.h"

PFILA2 running, blocked, finished; // Filas comuns para executando, bloqueados e terminados

PFILAPRIO filaPrioridades; // Fila de prioridades para aptos

int tid = 0; // ID de threads

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


int insertFilaPrioridades(ucontext_t context, int priority) {
    tid += 1;
    int state = 1;

    TCB_t * content = malloc(sizeof(TCB_t));
    *content = (TCB_t) {.tid = tid, .state = state, .prio = priority, .context = context};

    switch (priority) {
        case 0:
            AppendFila2(filaPrioridades->high, content);
            break;
        case 1:
            AppendFila2(filaPrioridades->medium, content);
            break;
        case 2:
            AppendFila2(filaPrioridades->low, content);
            break;
        default:
            return -1;

    }

    return 0;

}

int initStdFila(PFILA2 fila){
    if (fila == NULL) {
        fila = malloc(sizeof(PFILA2));
        CreateFila2(fila);
    }
    return 0;
}

