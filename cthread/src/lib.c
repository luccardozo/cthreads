
#include <stdio.h>
#include <string.h>
#include <ucontext.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"


int ccreate (void* (*start)(void*), void *arg, int prio) {
	return -1;
}

int csetprio(int tid, int prio) {
	return -1;
}

int cyield(void) {
	return -1;
}

int cjoin(int tid) {
	return -1;
}

int csem_init(csem_t *sem, int count) {
	return -1;
}

int cwait(csem_t *sem) {
	return -1;
}

int csignal(csem_t *sem) {
	return -1;
}

int cidentify (char *name, int size) {
	strncpy (name, "Victor de Almeida Piccoli Ferreira - 221192\nLucas Cardoso Tavares - 264411\nSamuel Rudnicki - 214871", size);
	return 0;
}


