#ifndef __scheduler__
#define __scheduler__
#include "../include/support.h"
#include "../include/cdata.h"

// Fila de prioridades
typedef struct FPrio {
    PFILA2 high;
    PFILA2 medium;
    PFILA2 low;
} FILAPRIO;

void createFilaPrioridades(FILAPRIO *filaPrioridades);

int insertFilaPrioridades(FILAPRIO *filaPrioridades, int priority);

#endif
