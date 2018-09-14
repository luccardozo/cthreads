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

    content = findThread(1);

    content = findThread(2);

    return 0;
}