#include "front.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void List_Init(List *list) {
    if (list == NULL)
        return;

    list->activeElement = NULL;
    list->firstElement = NULL;
    list->currentLength = 0;
}

void List_Dispose(List *list) {
    if (list == NULL) {
        return;
    }

    ListElementPtr tmp_elem;

    while (list->firstElement != NULL) {
        tmp_elem = list->firstElement;          
        list->firstElement = list->firstElement->nextElement; 
        free(tmp_elem->data); // Free the data if dynamically allocated
        free(tmp_elem);                          
    }

    list->activeElement = NULL;
    list->firstElement = NULL;
    list->currentLength = 0;
}

void List_Insert(List *list, stackSymbol *data) {
    if (list == NULL) {
        return;
    }

    ListElementPtr newElement = malloc(sizeof(struct ListElement));
    if (newElement == NULL) {
         error_exit(ERR_MALLOC, "Malloc failed in List_Insert [front.c]");
        return;
    }

    newElement->data = data;

    if (list->activeElement == NULL) { // Insert at the beginning
        newElement->nextElement = list->firstElement; 
        list->firstElement = newElement;
    } else { // Insert after the active element
        newElement->nextElement = list->activeElement->nextElement;
        list->activeElement->nextElement = newElement;
    }
    
    list->currentLength++;
}

void List_First(List *list) {
    list->activeElement = list->firstElement;
}

stackSymbol* List_Get_First(List *list) { // Change to stackSymbol**
    if (list == NULL || list->firstElement == NULL) {
        error_exit(ERR_MALLOC, "Malloc failed in List_Get_first [front.c]");
    }

    return list->firstElement->data;
}

void List_Delete_First(List *list) {
    if (list->firstElement == NULL)
        return;

    ListElementPtr newFirst = list->firstElement->nextElement;
    free(list->firstElement->data); // Free the data if dynamically allocated
    free(list->firstElement);
    if (list->activeElement == list->firstElement) {
        list->activeElement = NULL; 
    }
    list->firstElement = newFirst;
    list->currentLength--;
}


void List_SetValue(List *list, stackSymbol *data) { // Change to stackSymbol*
    if (list->activeElement == NULL || list == NULL)
        return;

    list->activeElement->data = data;
}

void List_Next(List *list) {
    if (list == NULL || list->activeElement == NULL) {
        return; 
    }
    list->activeElement = list->activeElement->nextElement;
}

int List_IsActive(List *list) {
    return (list != NULL && list->activeElement != NULL) ? 1 : 0;
}