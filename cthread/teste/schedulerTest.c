#include <stdio.h>
#include <stdlib.h>
#include "../include/scheduler.h"

int main() {
    TCB_t *content = malloc(sizeof(TCB_t));
    ucontext_t context;
    initMainThread();

    getcontext(&context);

    content = createThread(context,FPRIO_PRIORITY_LOW);
    insertFilaPrioridades(content);

    content = findThread(0);

    printf("\nFINDTHREAD - { tid = %d, state = %d, prio = %d, context = %p}\n", content->tid, content->state, content->prio, &content->context);

    content = findThread(1);

    printf("\nFINDTHREAD - { tid = %d, state = %d, prio = %d, context = %p}\n", content->tid, content->state, content->prio, &content->context);

    content = findThread(2);

    printf("\nFINDTHREAD - %p - deve ser NULL\n", content);

    return 0;
}