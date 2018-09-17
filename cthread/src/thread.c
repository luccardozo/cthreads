#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/scheduler.h"
#include "../include/cdata.h"
#include "../include/thread.h"
#include "../include/cthread.h"

void createContext(ucontext_t * context, void* (*start)(void*)){
/*Contexto de saida, será chamado quando a Nova Thread terminar e então chama a função de escolha do escalonador*/
    ucontext_t * exitContext = malloc(sizeof(ucontext_t));
    getcontext(exitContext);
    exitContext->uc_stack.ss_sp = malloc(STACK_SIZE);
    exitContext->uc_stack.ss_size = STACK_SIZE;
    exitContext->uc_stack.ss_flags = 0;
    context->uc_link = 0;
    makecontext(exitContext, (void*)chooseAndRunReadyThread, 0);

/*Contexto da Nova Thread*/
    //alocamento do contexto
    getcontext(context);
    context->uc_stack.ss_sp = malloc(STACK_SIZE);
    context->uc_stack.ss_size = STACK_SIZE;
	context->uc_stack.ss_flags = 0;
    //contexto de retorno -> tem que retornar para o escalonador
    context->uc_link = exitContext;
    makecontext(context, (void*)start, 0);
}