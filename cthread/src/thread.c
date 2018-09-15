#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/scheduler.h"
#include "../include/cdata.h"
#include "../include/thread.h"
#include "../include/cthread.h"

#define STACK_SIZE 1024*32

//passar o controle para o sheduler
ucontext_t scheduler;

void createContext(ucontext_t * context, void* (*start)(void*)){
/*Contexto de saida, será chamado quando a Nova Thread terminar e então chama a função de escolha do escalonador*/
    ucontext_t * exitContext = malloc(sizeof(ucontext_t));
    getcontext(exitContext);
    exitContext->uc_stack.ss_sp = malloc(STACK_SIZE);
    exitContext->uc_stack.ss_size = STACK_SIZE;
    exitContext->uc_stack.ss_flags = 0;
    context->uc_link = 0;
    makecontext(exitContext, (void*)&chooseAndRunReadyThread, 0);

/*Contexto da Nova Thread*/
    //alocamento do contexto
    getcontext(context);
    context->uc_stack.ss_sp = malloc(STACK_SIZE);
    context->uc_stack.ss_size = STACK_SIZE;
    //contexto de retorno -> tem que retornar para o escalonador **arrumar**
    context->uc_link = exitContext;
    makecontext(context, (void*)start, 0);
}
/*
int allocExitContext()
{
	exitContext = (ucontext_t*) malloc(sizeof(ucontext_t));
	if(exitContext == NULL) 
        return ERROR; // erro de alocacao de memoria

	exitContext->uc_link = NULL;
	exitContext->uc_stack.ss_sp = (char*) malloc(sizeof(SIGSTKSZ));
		
	if(exitContext->uc_stack.ss_sp == NULL) // erro de alocacao de memoria
		return ERROR;
	
	exitContext->uc_stack.ss_size = SIGSTKSZ;
	// preenche outros campos necessarios que nao sao
	// muito importantes para o escalonamento
	// a partir do contexto atual
	getcontext(exit_context);

	// quando chamado (ou seja, ao final da execucao)
	// de cada thread, o contexto de saida
	// deve chamar a funcao de termino de thread,
	// do escalonador
	makecontext(exit_context, (void (*)(void)) terminateThread, 0, NULL);
	return SUCCESS;
}
*/