#ifndef __thread__
#define __thread__
#include "../include/scheduler.h"
#include "../include/support.h"
#include "../include/cdata.h"

#define TRUE = 1
#define FALSE = 0

#define SUCCESS 0
#define ERROR -1
#define STACK_SIZE 1024*32

void createContext(ucontext_t * context, void* (*start)(void*));
//int allocExitContext();

#endif