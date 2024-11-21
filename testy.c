#include "testy.h"
#include <stdio.h>
#include <stdlib.h> 

void PrintStack(TStack *stack) {
    TSElemPtr tselem = stack->top;
    printf("    ------------TOP OF STACK---------------\n");
    while (tselem != NULL) {
        printf("    isTerm: %d, stackSymbol value: %d\n", ((stackSymbol *)tselem->data)->isTerm, ((stackSymbol *)tselem->data)->value); // Výpis hodnôt
        tselem = tselem->next;
    }
    printf("    ------------BOTTOM OF STACK------------\n");
}

void write_token_type(Token token) {
    switch (token.type) {
        case TOKEN_EOF:
            printf("TOKEN_EOF");
            break;
        case TOKEN_STRING_LITERAL:
            printf("TOKEN_STRING_LITERAL: %s", token.value.arr.stringValue);
            //free_array(&token.value.arr);
            break;
        case TOKEN_INTEGER_LITERAL:
            printf("TOKEN_INTEGER_LITERAL: %d", token.value.intValue);
            break;
        case TOKEN_FLOAT_LITERAL:
            printf("TOKEN_FLOAT_LITERAL: %f", token.value.floatValue);
            break;
        case TOKEN_IDENTIFIER:
            printf("TOKEN_IDENTIFIER: %s", token.value.arr.stringValue);
            //free_array(&token.value.arr);
            break;
        case TOKEN_COMPTIME:
            printf("TOKEN_COMPTIME: %s", token.value.arr.stringValue);
            //free_array(&token.value.arr);
            break;
        case TOKEN_EQUAL:
            printf("TOKEN_EQUAL: ==");
            break;
        case TOKEN_NOT_EQUAL:
            printf("TOKEN_NOT_EQUAL: !=");
            break;
        case TOKEN_LESS:
            printf("TOKEN_LESS: <");
            break;
        case TOKEN_GREATER:
            printf("TOKEN_GREATER: >");
            break;
        case TOKEN_LESS_EQUAL:
            printf("TOKEN_LESS_EQUAL: <=");
            break;
        case TOKEN_GREATER_EQUAL:
            printf("TOKEN_GREATER_EQUAL: >=");
            break;
        case TOKEN_ASSIGNMENT:
            printf("TOKEN_ASSIGNMENT: =");
            break;
        case TOKEN_PLUS:
            printf("TOKEN_PLUS: +");
            break;
        case TOKEN_MINUS:
            printf("TOKEN_MINUS: -");
            break;
        case TOKEN_MULTIPLY:
            printf("TOKEN_MULTIPLY: *");
            break;
        case TOKEN_DIVIDE:
            printf("TOKEN_DIVIDE: /");
            break;
        case TOKEN_PIPE:
            printf("TOKEN_PIPE: |");
            break;
        case TOKEN_COMMA:
            printf("TOKEN_COMMA: ,");
            break;
        case TOKEN_Q_MARK:
            printf("TOKEN_Q_MARK: ?");
            break;
        case TOKEN_DOT:
            printf("TOKEN_DOT: .");
            break;
        case TOKEN_LEFT_PAREN:
            printf("TOKEN_LEFT_PAREN: (");
            break;
        case TOKEN_RIGHT_PAREN:
            printf("TOKEN_RIGHT_PAREN: )");
            break;
        case TOKEN_LEFT_BRACE:
            printf("TOKEN_LEFT_BRACE: {");
            break;
        case TOKEN_RIGHT_BRACE:
            printf("TOKEN_RIGHT_BRACE: }");
            break;
        case TOKEN_ARRAY_BRACE:
            printf("TOKEN_RIGHT_BRACE: []");
            break;
        case TOKEN_SEMICOLON:
            printf("TOKEN_SEMICOLON: ;");
            break;
        case TOKEN_COLON:
            printf("TOKEN_COLON: :");
            break;
        case TOKEN_IF:
            printf("TOKEN_IF: if");
            break;
        case TOKEN_ELSE:
            printf("TOKEN_ELSE: else");
            break;
        case TOKEN_RETURN:
            printf("TOKEN_RETURN: return");
            break;
        case TOKEN_WHILE:
            printf("TOKEN_WHILE: while");
            break;
        case TOKEN_FN:
            printf("TOKEN_FN: fn");
            break;
        case TOKEN_CONST:
            printf("TOKEN_CONST: const");
            break;
        case TOKEN_PUB:
            printf("TOKEN_PUB: pub");
            break;
        case TOKEN_VOID:
            printf("TOKEN_VOID: void");
            break;
        case TOKEN_VAR:
            printf("TOKEN_VAR: var");
            break;
        case TOKEN_I32:
            printf("TOKEN_I32: i32");
            break;
        case TOKEN_F64:
            printf("TOKEN_F64: f64");
            break;
        case TOKEN_NULL:
            printf("TOKEN_NULL: null");
            break;
        case TOKEN_U8:
            printf("TOKEN_U8: u8");
            break;
        case TOKEN_UNKNOWN:
            printf("TOKEN_UNKNOWN: unknown token");
            break;
        default:
            printf("Unknown token type.");
            break;
    
    }
}

void get_non_term_name(NonTerm nonTerm) {
    switch (nonTerm) {
        case N_PROGRAM:
            printf("N_PROGRAM");
	    break;
        case N_FUNCTION:
            printf("N_FUNCTION");
	    break;
        case N_FN_PARAMS:
            printf("N_FN_PARAMS");
	    break;
        case N_STATMENT:
            printf("N_STATMENT");
	    break;
        case N_CLEAR:
            printf("N_CLEAR");
	    break;
        case N_NEXT_VAR:
            printf("N_NEXT_VAR");
	    break;
        case N_STATMENT_ID:
            printf("N_STATMENT_ID");
	    break;
        case N_SPLITER:
            printf("N_SPLITER");
	    break;
        case N_FUNCTION_CALL:
            printf("N_FUNCTION_CALL");
	    break;
        case N_FN_CALL_NEXT:
            printf("N_FN_CALL_NEXT");
	    break;
        case N_IS_NULL:
            printf("N_IS_NULL");
	    break;
        case N_ELSE:
            printf("N_ELSE");
	    break;
        case N_TYPE:
            printf("N_TYPE");
	    break;
        case N_LITERALS:
            printf("N_LITERALS");
	    break;
        case N_EXPRESION:
            printf("N_EXPRESION");
	    break;
        case N_NEXT_LIT:
            printf("N_NEXT_LIT");
	    break;
        case N_OPERATOR:
            printf("N_OPERATOR");
	    break;
        case N_FNCALL_TYPE:
            printf("N_FNCALL_TYPE");
	    break;
        case N_FN_CALL_COMA:
            printf("N_FN_CALL_COMA");
	    break;
        case N_QUESTION:
            printf("N_QUESTION");
	    break;
        case N_ARRAY_BRACE:
            printf("N_ARRAY_BRACE");
	    break;
        case N_TYPE_PREFIX:
            printf("N_TYPE_PREFIX");
	    break;
        case __NONTERMEND__:
            printf("__NONTERMEND__");
	    break;
        default:
            printf("Neznámý neterminál");
	    break;
    }
}

void print_LLtable (Token token, NonTerm NonTerm){
    printf("    LL_TABLE = [");
    get_non_term_name(NonTerm);
    printf(", ");
    write_token_type(token);
    printf("]\n");
}

void List_Print(List *list) {
    if (list == NULL || list->firstElement == NULL) {
        printf("List is empty.\n");
        return;
    }

    
    printf("List contents:\n");
    while (list->firstElement != NULL) {
        // Assuming stackSymbol has a field called 'name' for printing
        // Adjust this based on your actual stackSymbol structure
        stackSymbol* current = List_Get_First(list);
        if (current!= NULL) {
            printf("isTerm: %d, value: %d\n", current->isTerm, current->value); 
        }
        List_Delete_First(list);
    }
}