#include <stdio.h>
#include <string.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/scheduler.h"
#include "../include/cdata.h"


void createFilaPrioridades(FILAPRIO *filaPrioridades) {
    if(filaPrioridades->high == NULL) {
        filaPrioridades->high = malloc(sizeof(PFILA2));
        CreateFila2(filaPrioridades->high);
    }
    if(filaPrioridades->medium == NULL) {
        filaPrioridades->medium = malloc(sizeof(PFILA2));
        CreateFila2(filaPrioridades->medium);
    }
    if(filaPrioridades->low == NULL) {
        filaPrioridades->low = malloc(sizeof(PFILA2));
        CreateFila2(filaPrioridades->low);
    }
    return;
}


int insertFilaPrioridades(FILAPRIO *filaPrioridades, int priority) {
    TCB_t * content = malloc(sizeof(TCB_t));
    switch (priority) {
        case 0:
            AppendFila2(filaPrioridades->high, content);
        case 1:
            AppendFila2(filaPrioridades->medium, content);
        case 2:
            AppendFila2(filaPrioridades->low, content);
        default:
            return -1;

    }

    return 0;

}

