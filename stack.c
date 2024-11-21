#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Inicializuje zásobník
void stack_init(TStack *stack) {
    stack->top = NULL;
}

// Přidá nový prvek na vrchol zásobníku
void Push(TStack *stack, void *data) {
    TSElemPtr newElem = (TSElemPtr)malloc(sizeof(struct TSElem));
    if (newElem == NULL) {
        fprintf(stderr, "Error: Memory allocation failed in Push\n");
        exit(EXIT_FAILURE);
    }
    newElem->data = data;      // Ukládá obecná data
    newElem->next = stack->top;
    stack->top = newElem;
}

// Odebere prvek z vrcholu zásobníku
void Pop(TStack *stack, void (*freeFunc)(void *)) {
    if (!IsEmpty(stack)) {
        TSElemPtr elemPtr = stack->top;
        stack->top = stack->top->next;

        // Pop neví, zda má data uvolňovat a už vůbec neví jak, předáme mu funkci na uvolnění dat (pokud existuje).
        // Uvolnění uložených dat (pokud je předána funkce pro uvolnění)
        if (freeFunc) { //Jestliže existuje funkce na uvolnění
            freeFunc(elemPtr->data);
        }
        free(elemPtr);
    } else {
        fprintf(stderr, "Error: Pop operation on empty stack\n");
        exit(EXIT_FAILURE);
    }
}

// Vrací prvek z vrcholu zásobníku bez jeho odebrání
void *Top(TStack *stack) {
    if (IsEmpty(stack)) {
        fprintf(stderr, "Error: Top operation on empty stack\n");
        exit(EXIT_FAILURE);
    }
    return stack->top->data;
}

// Kontroluje, zda je zásobník prázdný
bool IsEmpty(TStack *stack) {
    return (stack->top == NULL);
}

// Uvolní celý zásobník
void stack_dtor(TStack *stack, void (*freeFunc)(void *)) {
    while (!IsEmpty(stack)) {
        Pop(stack, freeFunc);
    }
}
