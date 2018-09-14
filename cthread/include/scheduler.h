#ifndef __scheduler__
#define __scheduler__
#include "../include/support.h"
#include "../include/cdata.h"

#define FPRIO_STATE_CREATION 0
#define FPRIO_STATE_READY 1
#define FPRIO_STATE_EXECUTING 2
#define FPRIO_STATE_BLOCKED 3
#define FPRIO_STATE_FINISHED 4

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

void createFilaPrioridades(FILAPRIO *filaPrioridades);

int insertFilaPrioridades(FILAPRIO *filaPrioridades, int priority);

#endif
