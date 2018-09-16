#ifndef __thread__
#define __thread__
#include "../include/scheduler.h"
#include "../include/support.h"
#include "../include/cdata.h"

#define TRUE = 1
#define FALSE = 0

#define SUCCESS 0
#define ERROR -1

/*Estrutura para a fila de JOINS, para a função CJOINS*/
typedef struct joins{
    int pid_thread;
    int pid_threadWaiting;
} joint;

void createContext(ucontext_t * context, void* (*start)(void*));
//int allocExitContext();

#endif