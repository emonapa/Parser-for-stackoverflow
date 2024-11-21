#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* Types of errors */
typedef enum {
    ERR_LEXICAL = 1,            // Lexical error
    ERR_SYNTAX = 2,             // Syntax error

    // Semantic errors
    ERR_NOT_DEFINED = 3,        // Undefined variable or function
    ERR_TYPE_NUM_FUN = 4,       // Incorrect number of function parameters
    ERR_RESULT_THROWN_FUN = 4,  // Incorrect function return result
    ERR_REDEFINITION = 5,       // Redefinition of variable or function
    ERR_CONST_MODIFIED = 5,     // Modification of a constant
    ERR_BAD_RETURN_FUN = 6,     // Incorrect return type in function
    ERR_TYPE_NOT_MATCHING = 7,  // Type mismatch
    ERR_VAR_IRREDUCTIBLE = 8,   // Variable cannot be reduced or processed
    ERR_VAR_NOT_USED = 9,       // Variable is declared but not used
    ERR_VAR_NOT_MODIFIED = 9,   // Variable is not modified
    ERR_OTHER_SEMANTIC = 10,    // Other semantic error

    ERR_MALLOC = 99,            // Memory allocation error
    ERR_EMPTY_STACK = 99,       // Empty stack error
    WRONG_TOKEN = 99,           // Unexpected token error
    CANT_ADD_TOKEN = 99,        // Cannot add token to structure
    ERR_OTHER = 99              // Other unspecified errors
} ERROR_CODES;

/* Function to print an error message and terminate the program */
void error_exit(ERROR_CODES err_code, const char *fmt, ...);

/* Function to print a warning message */
void warning(const char *fmt, ...);

#endif  /* ERROR_H */
