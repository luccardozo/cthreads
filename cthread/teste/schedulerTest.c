#include <stdio.h>
#include <stdlib.h>
#include "../include/scheduler.h"

int main() {
    ucontext_t context;
    getcontext(&context);
    createFilaPrioridades();
    

    insertFilaPrioridades(context, 0);

    return 0;
}