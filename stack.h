#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdbool.h>

// Typ ukazatele na data v zásobníku
typedef struct TSElem {
    void *data;               // Obecná data
    struct TSElem *next;      // Ukazatel na další prvek
} *TSElemPtr;

typedef struct {
    TSElemPtr top;            // Ukazatel na vrchol zásobníku
} TStack;

// Funkce pro práci se zásobníkem
void stack_init(TStack *);
void Push(TStack *, void *);
void Pop(TStack *, void (*)(void *)); // Funkce pro uvolnění dat (volitelná)
void *Top(TStack *);
bool IsEmpty(TStack *);
void stack_dtor(TStack *, void (*)(void *)); // Funkce pro uvolnění dat (volitelná)

#endif // STACK_H
