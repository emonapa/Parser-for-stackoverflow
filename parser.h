#ifndef PARSER_H
#define PARSER_H
#include "stack.h"
#include "lexer.h"
#include "ast.h"

typedef struct ast_node ast_node_t;

extern const int LL_TABLE[22][43];

typedef enum {
    N_PROGRAM = __TOKENEND__ + 1, 
    N_FUNCTION,
    N_FN_PARAMS,
    N_STATMENT,
    N_CLEAR,
    N_NEXT_VAR,
    N_STATMENT_ID,
    N_SPLITER,
    N_FUNCTION_CALL,
    N_FN_CALL_NEXT,
    N_IS_NULL,
    N_ELSE,
    N_TYPE,
    N_LITERALS,
    N_EXPRESION, 
    N_NEXT_LIT,
    N_OPERATOR,
    N_FNCALL_TYPE,
    N_FN_CALL_COMA,
    N_QUESTION,
    N_ARRAY_BRACE,
    N_TYPE_PREFIX,
    __NONTERMEND__
} NonTerm;

typedef struct {
    int isTerm;  // 1 pre terminál, 0 pre neterminál
    int value;
} stackSymbol;

void free_stackSymbol(void *data);
void Push_T_NT(TStack *stack, int value);
void processValue(int value, TStack *stack);
bool find_rule(LexerContext *context, TStack *stack, ast_node_t *tree);

#endif // PARSER_H