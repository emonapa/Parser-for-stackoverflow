
#ifndef FRONT_H
#define FRONT_H

#include <stdbool.h> // For bool type
#include "parser.h"
#include <stdio.h>

typedef struct ListElement {
    stackSymbol *data; // Change from int to stackSymbol*
    struct ListElement *nextElement;
} *ListElementPtr;

typedef struct {
    ListElementPtr firstElement;
    ListElementPtr activeElement;
    int currentLength;
} List;

void List_Init(List *list);
void List_Dispose(List *list);
void List_Insert(List *list, stackSymbol *data); // Change to stackSymbol*
void List_First(List *list);
stackSymbol* List_Get_First(List *list); 
void List_Delete_First(List *list);
void List_SetValue(List *list, stackSymbol *data); // Change to stackSymbol*
void List_Next(List *list);
int List_IsActive(List *list);

#endif // FRONT_H