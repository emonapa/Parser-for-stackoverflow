#include "error.h"

/* Funkcia pre výpis varovania */
void warning(const char *fmt, ...) {
    va_list args;

    // Vypíše predponu pre varovanie
    fprintf(stderr, "Warning: ");

    // Formátovaný výpis varovania
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

/* Function to print an error message and terminate the program */
void error_exit(ERROR_CODES err_code, const char *fmt, ...) {
    va_list args;

    /* Formatted error message */
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    /* Terminate the program with the error code */
    exit(err_code);
}
