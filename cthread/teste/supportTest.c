#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"

int main() {
    PFILA2 fila;
    fila = malloc(sizeof(PFILA2)); // aloca espaco para fila
    int *content = malloc(sizeof(int));
    *content = 2;
    printf("\nworked\n");
    CreateFila2(fila); // cria fila
    AppendFila2(fila, content); // coloca content na fila
    FirstFila2(fila); // coloca o iterador na primeira posicao da fila
    printf("\nEndereco int = %p\n", content);
    printf("\nEstrutura da fila = {it = %p, first = %p, last = %p }\n", fila->it, fila->first, fila->last);
    printf("\nIterador = { node = %p, ant = %p, next = %p }\n", fila->it->node, fila->it->ant, fila->it->next);
    printf("\n%d - era o numero\n", *(int*)GetAtIteratorFila2(fila)); // typecast para variavel
    DeleteAtIteratorFila2(fila);
    return 0;
}