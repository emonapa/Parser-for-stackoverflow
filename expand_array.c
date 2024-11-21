#include <stdlib.h>
#include <string.h>
#include "expand_array.h"
#include "lexer.h"

// Funkce pro inicializaci dynamického pole
char *init_array(DynamicCharArray *arr) {
    arr->size = 0;
    arr->capacity = 2;  // Začínáme s kapacitou 2 znaků
    arr->stringValue = (char *)malloc(arr->capacity * sizeof(char)); // Alokujeme paměť
    if (arr->stringValue == NULL) {
        printf("Alokace paměti selhala!\n");
        exit(1);
    }
    return arr->stringValue;
}

// Funkce pro uvolnění paměti
void free_array(DynamicCharArray *arr) {
    free(arr->stringValue); // Uvolníme dynamicky alokované pole
    arr->stringValue = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

void add_char(DynamicCharArray *arr, int c) {
    // Zkontrolujeme, zda je pole plné
    if (arr->size >= arr->capacity) {
        // Zvětšíme kapacitu na dvojnásobek
        arr->capacity *= 2;
        arr->stringValue = (char *)realloc(arr->stringValue, arr->capacity * sizeof(char));
        if (arr->stringValue == NULL) {
            printf("Realokace paměti selhala!\n");
            exit(1);
        }
    }
    // Přidáme znak do pole
    arr->stringValue[arr->size++] = c;
}


// Funkce pro vypsání obsahu pole
void print_array(const DynamicCharArray *arr) {
    for (size_t i = 0; i < arr->size; i++) {
        printf("%c", arr->stringValue[i]);
    }
    printf("\n");
}