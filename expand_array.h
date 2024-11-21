#ifndef EXPAND_ARRAY
#define EXPAND_ARRAY

#include <stdio.h>

// Definice struktury pro nafukovací pole
typedef struct {
    char *stringValue;  // Ukazatel na dynamicky alokované pole znaků
    size_t size;        // Aktuální počet znaků v poli
    size_t capacity;    // Kapacita pole (kolik znaků může aktuálně obsahovat)
} DynamicCharArray;

// Funkce pro inicializaci dynamického pole
char *init_array(DynamicCharArray *arr);

// Funkce pro uvolnění paměti
void free_array(DynamicCharArray *arr);

// Funkce pro přidání znaku do nafukovacího pole
void add_char(DynamicCharArray *arr, int c);

// Funkce pro vypsání obsahu pole
void print_array(const DynamicCharArray *arr);

#endif // EXPAND_ARRAY