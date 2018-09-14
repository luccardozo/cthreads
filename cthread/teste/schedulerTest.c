#include <stdio.h>
#include <stdlib.h>
#include "../include/scheduler.h"

int main() {
    PFILAPRIO filaPrioridades = malloc(sizeof(PFILAPRIO));
    createFilaPrioridades(filaPrioridades);
    insertFilaPrioridades(filaPrioridades, 0);

    return 0;
}