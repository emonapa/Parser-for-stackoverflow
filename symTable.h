#ifndef LINKEDSYMTABLE_H
#define LINKEDSYMTABLE_H

#include <stdbool.h>

typedef enum {
    INT,
    FLOAT,
    STRING
} types;

// Structure for a symbol
typedef struct {
    char *name;
    types type;
    bool IsConstant;          // Indicates if the symbol is a constant
    bool UsedFlag;            // If false when the table is popped, throw error 9
    bool ChangedFlag;         // Indicates if the symbol was changed
} Symbol;

// Structure for a function symbol
typedef struct {
    char *name;         // Function name
    types returnType;   // Return type
    int paramCount;     // Number of parameters
    types *paramTypes;  // Parameter types
} FunSymbol;

// Structure for the dynamic symbol table
typedef struct SymTable {
    struct SymTable *prev; // Pointer to the previous table
    Symbol **symTable;     // Array of pointers to symbols
    int capacityIndex;     // Index in the PRIMES array
    int size;              // Number of currently stored symbols
} SymTable;

// Structure for the dynamic function table
typedef struct {
    FunSymbol **funTable; // Array of pointers to functions
    int capacityIndex;    // Index in the PRIMES array
    int size;             // Number of currently stored functions
} FunTable;

// Linked symbol table
typedef struct {
    SymTable *top;      // Symbol table
    FunTable funTable;  // Function table
} LinkedSymTable;

// Function to initialize the table
void initTable(LinkedSymTable *linkedSymTable);

// Hash function
unsigned int hash(const char *str);

// Enter/exit a new scope
void enterScope(LinkedSymTable *linkedSymTable);
void exitScope(LinkedSymTable *linkedSymTable);

// Functions for working with symbols
void addSymbol(SymTable *symTable, const char *name, types type, bool IsConstant, bool UsedFlag, bool ChangedFlag);
Symbol *findSymbol(LinkedSymTable *linkedSymTable, const char *name);

// Functions for working with functions
void addFunction(LinkedSymTable *table, const char *name, types returnType, int paramCount, const types *paramTypes);
FunSymbol *findFunction(LinkedSymTable *table, const char *name);

// Function to destroy the table
void destroySymbolTable(LinkedSymTable *linkedSymTable);

#endif // LINKEDSYMTABLE_H
