#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symTable.h"
#include "error.h"

// For length of hash table
static const int PRIMES[] = {11, 23, 47, 97, 197, 397, 797, 1597};
static const int PRIMES_COUNT = sizeof(PRIMES) / sizeof(PRIMES[0]);

char *copyString(const char *src) {
    char *dest = malloc(strlen(src) + 1); // Alokace paměti pro řetězec
    if (dest == NULL) {error_exit(ERR_MALLOC, "Malloc failed in copyString [symTable.c]\n");}
    strcpy(dest, src); // Kopírování obsahu zdroje do cíle
    return dest;       // Vrací nový řetězec
}

// Hashovací funkce
unsigned int hash(const char *str) {
    assert(str != NULL);
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash;
}

/**
 * @brief Enters a new scope in the linked symbol table.
 * @param linkedSymTable linked symbol table
 */
void enterScope(LinkedSymTable *linkedSymTable) {
    assert(linkedSymTable != NULL);

    // Allocate a new symbol table
    SymTable *newTable = malloc(sizeof(SymTable));
    if (newTable == NULL) {
        error_exit(ERR_MALLOC, "Malloc failed in enterScope [symTable.c]\n");
    }

    // Initialize the new symbol table
    newTable->capacityIndex = 0; // Start with the smallest prime number
    newTable->size = 0;
    newTable->symTable = calloc(PRIMES[newTable->capacityIndex], sizeof(Symbol *));
    if (newTable->symTable == NULL) {
        free(newTable);
        error_exit(ERR_MALLOC, "Calloc failed in enterScope [symTable.c]");
    }

    // Connect the new table to the old one
    newTable->prev = linkedSymTable->top;
    linkedSymTable->top = newTable;
}


/**
 * @brief Exits the current scope in the linked symbol table.
 * @param linkedSymTable linked symbol table
 *
 * This function frees all the symbols in the current scope and
 * moves to the previous scope.
 */
void exitScope(LinkedSymTable *linkedSymTable) {
    assert(linkedSymTable != NULL);

    // Checking for semantic error 9
    Symbol *current;
    for (int i = 0; i < PRIMES[linkedSymTable->top->capacityIndex]; i++) {
        current = linkedSymTable->top->symTable[i];
        if (current != NULL) {
            if (current->UsedFlag == false) {
                error_exit(ERR_VAR_NOT_USED, "Unused variable: %s\n", current->name);
            } else if (!current->ChangedFlag && !current->IsConstant) {
                error_exit(ERR_VAR_NOT_MODIFIED, "Unmodified variable: %s\n", current->name);
            }
        }
    }

    // If the current scope is not empty
    if (linkedSymTable->top != NULL) {
        SymTable *oldTable = linkedSymTable->top;
        linkedSymTable->top = linkedSymTable->top->prev;

        // Free all the symbols in the current scope
        for (int i = 0; i < PRIMES[oldTable->capacityIndex]; i++) {
            Symbol *symbol = oldTable->symTable[i];
            if (symbol != NULL) {
                // Free the dynamically allocated name of the symbol
                free(symbol->name);
                // Free the symbol itself
                free(symbol);
            }
        }

        // Free the array of symbol pointers
        free(oldTable->symTable);
        // Free the symbol table itself
        free(oldTable);
    }
}


/**
 * @brief Initializes the linked symbol table.
 * @param linkedSymTable linked symbol table
 *
 * Initializes the linked symbol table by setting its top to NULL
 * and initializing its function table.
 */
void initTable(LinkedSymTable *linkedSymTable) {
    assert(linkedSymTable != NULL);
    linkedSymTable->top = NULL;

    // Initialize the function table
    linkedSymTable->funTable.capacityIndex = 0;
    linkedSymTable->funTable.size = 0;
    linkedSymTable->funTable.funTable = calloc(PRIMES[0], sizeof(FunSymbol *));
    if (linkedSymTable->funTable.funTable == NULL) {error_exit(ERR_MALLOC, "Calloc failed in initTable [symTable.c]");}
}


/**
 * @brief Rehashes the symbol table when it is full.
 * @param symTable symbol table
 *
 * Rehashes the symbol table when it is full. It creates a new
 * array of symbol pointers with double the size of the old
 * array, rehashes all the symbols in the old array into the new
 * array, and then frees the old array.
 */
void resizeSymTable(SymTable *symTable) {
    if (symTable->capacityIndex + 1 >= PRIMES_COUNT) {
        fprintf(stderr, "Error: Exceeded maximum size of symbol table.\n");
        return;
    }

    // Calculate the new capacity
    int newCapacity = PRIMES[++symTable->capacityIndex];

    // Allocate a new array of symbol pointers
    Symbol **newTable = calloc(newCapacity, sizeof(Symbol *));
    if (newTable == NULL) {error_exit(ERR_MALLOC, "Calloc failed in resizeSymTable [symTable.c]");}

    // Calculate the old capacity
    int oldCapacity = PRIMES[symTable->capacityIndex - 1];

    // Rehash all the symbols in the old array into the new array
    for (int i = 0; i < oldCapacity; i++) {
        if (symTable->symTable[i] != NULL) {
            // Calculate the index of the symbol in the new array
            unsigned int index = hash(symTable->symTable[i]->name) % newCapacity;
            // Handle collisions by linear probing
            while (newTable[index] != NULL) {
                index = (index + 1) % newCapacity;
            }
            // Insert the symbol into the new array
            newTable[index] = symTable->symTable[i];
        }
    }

    // Free the old array
    free(symTable->symTable);
    // Set the new array as the current array
    symTable->symTable = newTable;
}


/**
 * Resizes the function table when it is full.
 *
 * @param funTable The function table to be resized.
 */
void resizeFunTable(FunTable *funTable) {
    if (funTable->capacityIndex + 1 >= PRIMES_COUNT) {
        fprintf(stderr, "Error: Exceeded maximum size of function table.\n");
        return;
    }

    // Calculate the new capacity
    int newCapacity = PRIMES[++funTable->capacityIndex];

    // Allocate a new array of function symbols
    FunSymbol **newTable = calloc(newCapacity, sizeof(FunSymbol *));
    if (newTable == NULL) {error_exit(ERR_MALLOC, "Calloc failed in resizeFunTable [symTable.c]");}

    // Calculate the old capacity
    int oldCapacity = PRIMES[funTable->capacityIndex - 1];

    // Rehash all the symbols in the old array into the new array
    for (int i = 0; i < oldCapacity; i++) {
        if (funTable->funTable[i] != NULL) {
            // Calculate the index of the symbol in the new array
            unsigned int index = hash(funTable->funTable[i]->name) % newCapacity;
            // Handle collisions by linear probing
            while (newTable[index] != NULL) {
                index = (index + 1) % newCapacity;
            }
            // Insert the symbol into the new array
            newTable[index] = funTable->funTable[i];
        }
    }

    // Free the old array
    free(funTable->funTable);
    // Set the new array as the current array
    funTable->funTable = newTable;
}

/**
 * @brief Adds a symbol to the symbol table.
 * 
 * This function inserts a symbol into the given symbol table. If the symbol
 * already exists, it updates the symbol's type. If the table is full, it resizes
 * the table before inserting the new symbol.
 * 
 * @param symTable The symbol table to insert the symbol into.
 * @param name The name of the symbol.
 * @param type The data type of the symbol.
 * @param IsConstant Flag indicating if the symbol is a constant.
 * @param UsedFlag Flag indicating if the symbol has been used.
 * @param ChangedFlag Flag indicating if the symbol has been changed.
 */
void addSymbol(SymTable *symTable, const char *name, types type, bool IsConstant, bool UsedFlag, bool ChangedFlag) {
    assert(symTable != NULL && name != NULL);

    // Resize the symbol table if it is more than half full
    if (symTable->size >= PRIMES[symTable->capacityIndex] / 2) {
        resizeSymTable(symTable);
    }

    int capacity = PRIMES[symTable->capacityIndex];
    unsigned int index = hash(name) % capacity;

    // Check if the symbol already exists and update its type if found
    while (symTable->symTable[index] != NULL) {
        if (strcmp(symTable->symTable[index]->name, name) == 0) {
            symTable->symTable[index]->type = type;
            return;
        }
        index = (index + 1) % capacity;
    }

    // Create a new symbol and insert it into the table
    Symbol *newSymbol = malloc(sizeof(Symbol));
    if (newSymbol == NULL) {
        error_exit(ERR_MALLOC, "Malloc failed in addSymbol [symTable.c]\n");
    }
    newSymbol->name = copyString(name); // Copy the symbol's name
    newSymbol->type = type;
    newSymbol->IsConstant = IsConstant;
    newSymbol->UsedFlag = UsedFlag;
    newSymbol->ChangedFlag = ChangedFlag;
    
    symTable->symTable[index] = newSymbol;
    symTable->size++;
}


/**
 * @brief Finds a symbol in the symbol table.
 * @param table The linked list of symbol tables.
 * @param name The name of the symbol to find.
 * @return The found symbol or NULL if not found.
 */
Symbol *findSymbol(LinkedSymTable *table, const char *name) {
    assert(table != NULL && name != NULL);

    // Start at the top of the stack
    SymTable *currentTable = table->top;

    // Iterate over all levels of the symbol table
    while (currentTable != NULL) {
        // Get the capacity of the current table
        int capacity = PRIMES[currentTable->capacityIndex];

        // Calculate the index of the symbol in the table
        unsigned int index = hash(name) % capacity;

        // Search the current table for the symbol
        while (currentTable->symTable[index] != NULL) {
            if (strcmp(currentTable->symTable[index]->name, name) == 0) {
                return currentTable->symTable[index]; // Symbol found
            }
            index = (index + 1) % capacity; // Linear probing
        }

        // Move to the previous level
        currentTable = currentTable->prev;
    }

    return NULL; // Symbol not found
}



/**
 * @brief Adds a function to the symbol table.
 * @param table The linked list of symbol tables.
 * @param name The name of the function to add.
 * @param returnType The return type of the function.
 * @param paramCount The number of parameters of the function.
 * @param paramTypes The types of the parameters of the function.
 */
void addFunction(LinkedSymTable *table, const char *name, types returnType, int paramCount, const types *paramTypes) {
    assert(table != NULL && name != NULL);

    // Resize the function table if it is more than half full
    if (table->funTable.size >= PRIMES[table->funTable.capacityIndex] / 2) {
        resizeFunTable(&table->funTable);
    }

    int capacity = PRIMES[table->funTable.capacityIndex];
    unsigned int index = hash(name) % capacity;

    // Check if the function already exists in the table
    while (table->funTable.funTable[index] != NULL) {
        if (strcmp(table->funTable.funTable[index]->name, name) == 0) {
            fprintf(stderr, "Error: Function %s already exists.\n", name);
            return;
        }
        index = (index + 1) % capacity; // Linear probing
    }

    // Create a new FunSymbol and insert it into the table
    FunSymbol *newFunction = malloc(sizeof(FunSymbol));
    if (newFunction == NULL) {
        error_exit(ERR_MALLOC, "Malloc failed in addFunction [symTable.c]\n");
    }
    newFunction->name = copyString(name); // Copy the name of the function
    newFunction->returnType = returnType;
    newFunction->paramCount = paramCount;
    newFunction->paramTypes = malloc(paramCount * sizeof(types));
    if (newFunction->paramTypes == NULL) {
        error_exit(ERR_MALLOC, "Malloc failed in addFunction [symTable.c]\n");
    }
    memcpy(newFunction->paramTypes, paramTypes, paramCount * sizeof(types));

    table->funTable.funTable[index] = newFunction;
    table->funTable.size++;
}


/**
 * @brief Finds a function in the function table.
 * 
 * This function searches for a function by name within the given linked
 * symbol table's function table. It uses a hash function to determine the
 * starting index and resolves collisions using linear probing.
 * 
 * @param table The linked symbol table containing the function table.
 * @param name The name of the function to find.
 * @return A pointer to the found function symbol, or NULL if not found.
 */
FunSymbol *findFunction(LinkedSymTable *table, const char *name) {
    assert(table != NULL && name != NULL);

    // Calculate the capacity of the function table
    int capacity = PRIMES[table->funTable.capacityIndex];
    // Calculate the initial index using the hash function
    unsigned int index = hash(name) % capacity;

    // Search through the function table using linear probing
    while (table->funTable.funTable[index] != NULL) {
        // Compare the function names to find a match
        if (strcmp(table->funTable.funTable[index]->name, name) == 0) {
            return table->funTable.funTable[index]; // Function found
        }
        // Move to the next index with wrap-around
        index = (index + 1) % capacity;
    }

    return NULL; // Function not found
}

// Destruktor tabulky
void destroySymbolTable(LinkedSymTable *linkedSymTable) {
    assert(linkedSymTable != NULL);

    // Uvolnění všech úrovní symbolických tabulek pomocí exitScope
    while (linkedSymTable->top != NULL) {
        exitScope(linkedSymTable);
    }

    // Uvolnění funkční tabulky (FunTable)
    for (int i = 0; i < PRIMES[linkedSymTable->funTable.capacityIndex]; i++) {
        if (linkedSymTable->funTable.funTable[i] != NULL) {
            free(linkedSymTable->funTable.funTable[i]->name);       // Uvolnění názvu funkce
            free(linkedSymTable->funTable.funTable[i]->paramTypes); // Uvolnění typů parametrů
            free(linkedSymTable->funTable.funTable[i]);             // Uvolnění samotné funkce
        }
    }

    free(linkedSymTable->funTable.funTable); // Uvolnění pole ukazatelů na funkce
}


