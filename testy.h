#ifndef TESTY
#define TESTY

#include "stack.h"
#include <stdio.h>
#include <stdlib.h> 
#include "parser.h"
#include "lexer.h"
#include "error.h"
#include "front.h"


void PrintStack(TStack *s);
void write_token_type(Token token);
void get_non_term_name(NonTerm nonTerm);
void print_LLtable (Token token, NonTerm NonTerm);
void List_Print(List* list);
#endif // TESTY
