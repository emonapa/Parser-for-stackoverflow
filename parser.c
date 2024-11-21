#include <stdlib.h>
#include <assert.h>
#include "lexer.h"
#include "expand_array.h"
#include "error.h"
#include "stack.h"
#include "parser.h"
#include "testy.h"
#include "symTable.h"

 const int LL_TABLE[22][43] = 
    { 
        {1, 2, 4, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,2},
        {0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {0, 0, 0, 0, 6, 0, 14, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {3, 3, 5, 0, 7, 0, 0, 0, 3, 0, 0, 22, 23, 24, 25, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 56, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 0,0,0},
        {0, 53, 0, 0, 53, 12, 0, 0, 53, 0, 0, 53, 53, 53, 53, 53, 0, 53, 32, 0, 0, 0, 0, 0, 0, 0, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 0, 0,0,0},
        {0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {0, 0, 0, 0, 9, 0, 15, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36, 37, 38, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 0,0,0},
        {0, 16, 0, 0, 10, 0, 16, 0, 16, 0, 0, 16, 16, 16, 16, 16, 16, 16, 0, 0, 16, 0, 0, 0, 0, 0, 40, 41, 42, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0,0,0},
        {0, 11, 0, 0, 11, 70, 11, 0, 11, 0, 0, 11, 11, 11, 11, 11, 11, 11, 0, 0, 11, 0, 0, 0, 0, 0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 0, 0,0,0},
        {0, 17, 0, 0, 17, 0, 17, 0, 17, 0, 0, 17, 17, 17, 17, 17, 17, 29, 0, 0, 17, 0, 0, 0, 0, 0, 0, 0, 0, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 0, 0,0,0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 0, 0,0,0},
        {0, 0, 0, 0, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 59, 58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {0, 0, 0, 0, 0, 0, 61, 0, 0, 0, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0},
        {0, 0, 0, 0, 0, 0, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 65, 65, 65, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,66,65,0},//
        {0, 0, 0, 0, 0, 0, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 67, 67, 67, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,67,68,0},//
        {0, 0, 0, 0, 0, 0, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 69, 69, 69, 69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,69,69,0}
    };

void free_stackSymbol(void *data) {
    free(data);
}

/**
 * @brief Pushes a new stackSymbol onto the stack, with the given value and
 *        whether it is a terminal or non-terminal.
 *
 * @param stack The stack to push onto.
 * @param value The value of the stackSymbol to push.
 */
 void Push_T_NT(TStack *stack, int value) {
    //0 ... __TOKENEND__ ... __TOKENEND__ + 1 ... __NONTERMEND__
    assert((value >= 0 && value < __NONTERMEND__) && (value != __TOKENEND__));

    //stackSymbol new_symbol;
    stackSymbol *new_symbol = (stackSymbol *)malloc(sizeof(stackSymbol));
    if (new_symbol == NULL) {
        fprintf(stderr, "chybna alokacia\n");
        exit(1);
    }
    
    int isTerm = (value >= 0 && value < __TOKENEND__);
    new_symbol->isTerm = isTerm;
    new_symbol->value = isTerm ? value : value - __TOKENEND__ - 1;

    //printf("Created stackSymbol:[%d,%d]\n", new_symbol->isTerm, new_symbol->value);

    Push(stack, new_symbol);
}



/**
 * @brief Processes a value and pushes the corresponding symbols to the stack.
 * 
 * @param value Value to be processed.
 * @param stack Stack to push the symbols to.
 */
void processValue(int value, TStack *stack) {
    Pop(stack, free_stackSymbol);
    switch(value) {
        case 0:
            // printf("co tu robis picus jebe\n");
            error_exit(ERR_SYNTAX, "zla syntax\n");
        break;
        //PROGRAM -> eof
        case 1:// uz otocene
         printf("p1\n");
            Push_T_NT(stack, TOKEN_EOF);  
            break;
        //PROGRAM -> STATMENT ; PROGRAM
        case 2:
         printf("p2\n");
            Push_T_NT(stack,N_PROGRAM);
            //Push_T_NT(stack,EOF);
            Push_T_NT(stack,N_STATMENT);
            break;
        //STATMENT -> ''
        case 3:
         printf("p3\n");
            // Push_T_NT(stack,TOKEN_SEMICOLON);
            break;
        //PROGRAM-> FUNCTION PROGRAM
        case 4:
         printf("p4\n");
            Push_T_NT(stack,N_PROGRAM);
            Push_T_NT(stack,N_FUNCTION);
            break;
        //FUNCTION -> pub fn token_id ( FN_PARAMS ) TYPE_PREFIX  { STATMENT }
        case 5:
         printf("p5\n");
            Push_T_NT(stack,TOKEN_RIGHT_BRACE);
            Push_T_NT(stack,N_STATMENT);
            Push_T_NT(stack,TOKEN_LEFT_BRACE);
            Push_T_NT(stack,N_TYPE_PREFIX);
            Push_T_NT(stack,TOKEN_RIGHT_PAREN);
            Push_T_NT(stack,N_FN_PARAMS);
            Push_T_NT(stack,TOKEN_LEFT_PAREN);
            Push_T_NT(stack,TOKEN_IDENTIFIER);
            Push_T_NT(stack,TOKEN_FN);
            Push_T_NT(stack,TOKEN_PUB);
            break;
        //FN_PARAMS -> token_id : TYPE_PREFIX  FN_PARAMS
        case 6:
         printf("p6\n");
            Push_T_NT(stack,N_FN_PARAMS); 
            Push_T_NT(stack,N_TYPE_PREFIX);
            Push_T_NT(stack,TOKEN_COLON);
            Push_T_NT(stack,TOKEN_IDENTIFIER);
            break;
        //STATMENT -> token_id NEXT_VAR STATMENT
        case 7:
         printf("p7\n");
            Push_T_NT(stack,N_STATMENT);
            Push_T_NT(stack,TOKEN_SEMICOLON);
            Push_T_NT(stack,N_NEXT_VAR);
            Push_T_NT(stack,TOKEN_IDENTIFIER);
            break;
        //STATMENT_ID -> token_id SPLITER
        case 8:
         printf("p8\n");
            Push_T_NT(stack,N_SPLITER);
            Push_T_NT(stack,TOKEN_IDENTIFIER);
            break;
        //FN_CALL_NEXT -> FN_CALL_TYPE FN_CALL_COMA FN_CALL_NEXT
        case 9:
         printf("p9\n");
            Push_T_NT(stack,N_FN_CALL_NEXT);
            Push_T_NT(stack,N_FN_CALL_COMA);
            Push_T_NT(stack,N_FNCALL_TYPE);
            break;
        //LITERALS -> token_id
        case 10:
         printf("p10\n");
            Push_T_NT(stack,TOKEN_IDENTIFIER);
            break;
        //EXPRESION -> LITERALS NEXT_LIT
        case 11:
         printf("p11\n");// uz otocene
            Push_T_NT(stack,N_NEXT_LIT);
            Push_T_NT(stack,N_LITERALS); // duplicita vola sa v s tatment ,program ale aj v kazdom statmente 
            break;
        //SPLITER -> FUNCTION_CALL
        case 12:// uz otocene
         printf("p12\n");
            Push_T_NT(stack,N_FUNCTION_CALL);
            break;
        //FUNCTION_CALL-> ( FN_CALL_NEXT )
        case 13:// uz otocene
         printf("p13\n");
            Push_T_NT(stack,TOKEN_RIGHT_PAREN);//
            Push_T_NT(stack,N_FN_CALL_NEXT);
            Push_T_NT(stack,TOKEN_LEFT_PAREN);
            break;
        //LITERALS
        case 14:
            printf("p14\n"); 
            break;
        //FN_CALL_NEXT -> ''
        case 15:
            printf("p15\n");
            break;
        //LITERALS -> ''
        case 16:
         printf("p16\n");
            break;
        //NEXT_LIT -> ''
        case 17:
         printf("p17\n");
            break;
        //IS_NULL -> ''
        case 18:
         printf("p18\n");
            break;
        //CLEAR -> : TYPE_PREFIX = STATMENT_ID
        case 19:// uz otocene
         printf("p19\n");
            Push_T_NT(stack,N_STATMENT_ID);
            Push_T_NT(stack,TOKEN_ASSIGNMENT);
            Push_T_NT(stack,N_TYPE_PREFIX);
            Push_T_NT(stack,TOKEN_COLON);
            break;
        //FN_PARAMS -> , token_id : TYPE_PREFIX   FN_PARAMS
        case 20:
         printf("p20\n");// uz otocene
            Push_T_NT(stack,N_FN_PARAMS);
            Push_T_NT(stack,N_TYPE_PREFIX);
            Push_T_NT(stack,TOKEN_COLON);
            Push_T_NT(stack,TOKEN_IDENTIFIER);
            Push_T_NT(stack,TOKEN_COMMA);
            break;
        //ELSE
        case 21:
         printf("p21\n");
            Push_T_NT(stack,N_FN_CALL_NEXT);
            Push_T_NT(stack,N_FNCALL_TYPE);
            Push_T_NT(stack,TOKEN_COMMA);  
            break;
        //STATMENT -> if ( EXPRESION ) IS_NULL { STATMENT } ELSE STATMENT
        case 22:
         printf("p22\n");
             Push_T_NT(stack,N_ELSE);
            Push_T_NT(stack,TOKEN_RIGHT_BRACE);
            Push_T_NT(stack,N_STATMENT);
            Push_T_NT(stack,TOKEN_LEFT_BRACE);
            Push_T_NT(stack,N_IS_NULL);
            Push_T_NT(stack,TOKEN_RIGHT_PAREN);
            Push_T_NT(stack,N_EXPRESION);
            Push_T_NT(stack,TOKEN_LEFT_PAREN);
            Push_T_NT(stack,TOKEN_IF);
            break;
        //STATMENT -> while ( EXPRESION ) { STATMENT } STATMENT
        case 23:
         printf("p23\n");// while 
            Push_T_NT(stack,N_STATMENT);
            Push_T_NT(stack,TOKEN_RIGHT_BRACE);
            Push_T_NT(stack,N_STATMENT);
            Push_T_NT(stack,TOKEN_LEFT_BRACE);
            Push_T_NT(stack,N_IS_NULL);
            Push_T_NT(stack,TOKEN_RIGHT_PAREN);
            Push_T_NT(stack,N_EXPRESION);
            Push_T_NT(stack,TOKEN_LEFT_PAREN);
            Push_T_NT(stack,TOKEN_WHILE);
            break;
        //STATMENT -> var token_id CLEAR STATMENT
        case 24:
         printf("p24\n");
            Push_T_NT(stack,N_STATMENT);
            Push_T_NT(stack,TOKEN_SEMICOLON);
            Push_T_NT(stack,N_CLEAR);
            Push_T_NT(stack,TOKEN_IDENTIFIER); 
            Push_T_NT(stack,TOKEN_VAR);
            break;
        //STATMENT -> const token_id CLEAR STATMENT
        case 25:
         printf("p25\n");
            Push_T_NT(stack,N_STATMENT);
            Push_T_NT(stack,TOKEN_SEMICOLON);
            Push_T_NT(stack,N_CLEAR);
            Push_T_NT(stack,TOKEN_IDENTIFIER); 
            Push_T_NT(stack,TOKEN_CONST);
            break;
        //STATMENT -> return EXPRESION STATMENT
        case 26:
         printf("p26\n");
            Push_T_NT(stack,N_STATMENT);
            Push_T_NT(stack,TOKEN_SEMICOLON);
            Push_T_NT(stack,N_EXPRESION); 
            Push_T_NT(stack,TOKEN_RETURN);
            break;
        //CLEAR -> = STATMENT_ID
        case 27:
         printf("p27\n");
            Push_T_NT(stack,N_STATMENT_ID);
            Push_T_NT(stack,TOKEN_ASSIGNMENT);
            break;
        //NEXT_VAR -> = STATMENT_ID
        case 28:
         printf("p28\n");
            Push_T_NT(stack,N_STATMENT_ID);
            Push_T_NT(stack,TOKEN_ASSIGNMENT);
            break;
        //NEXT_LIT -> OPERATOR LITERALS NEXT_LIT
        case 29:
         printf("p29\n");
            Push_T_NT(stack,N_NEXT_LIT);
            Push_T_NT(stack,N_LITERALS); 
            Push_T_NT(stack,N_OPERATOR);
            break;
        //OPERATOR -> =
        case 30:
         printf("p30\n");
            Push_T_NT(stack,TOKEN_ASSIGNMENT);
            break;
        //NEXT_VAR -> . token_id ( FN_CALL_NEXT ) //literals
        case 31:
         printf("p31\n");
            Push_T_NT(stack,TOKEN_RIGHT_PAREN);
            Push_T_NT(stack,N_FN_CALL_NEXT);
            Push_T_NT(stack,TOKEN_LEFT_PAREN);
            Push_T_NT(stack,TOKEN_IDENTIFIER);
            Push_T_NT(stack,TOKEN_DOT);
            break;
        //SPLITER -> . token_id ( FN_CALL_NEXT )
        case 32:
         printf("p32\n");
            Push_T_NT(stack,TOKEN_RIGHT_PAREN);
            Push_T_NT(stack,N_FN_CALL_NEXT);
            Push_T_NT(stack,TOKEN_LEFT_PAREN);
            Push_T_NT(stack,TOKEN_IDENTIFIER);
            Push_T_NT(stack,TOKEN_DOT);
            break;
        //STATMENT_ID -> @import ( LITERALS )
        case 33:
         printf("p33\n");
            Push_T_NT(stack,TOKEN_RIGHT_PAREN);
            Push_T_NT(stack,N_LITERALS);
            Push_T_NT(stack,TOKEN_LEFT_PAREN);
            Push_T_NT(stack,TOKEN_COMPTIME);
            break;
        //IS_NULL -> | EXPRESION |
        case 34:
         printf("p34\n");
           Push_T_NT(stack,TOKEN_PIPE);
            Push_T_NT(stack,N_EXPRESION);
            Push_T_NT(stack,TOKEN_PIPE);
            break;
        //ELSE -> else { STATMENT }
        case 35:
         printf("p35\n");
            Push_T_NT(stack,N_STATMENT);
            Push_T_NT(stack,TOKEN_RIGHT_BRACE);
            Push_T_NT(stack,N_STATMENT);
            Push_T_NT(stack,TOKEN_LEFT_BRACE);
            Push_T_NT(stack,TOKEN_ELSE);
            break;
        //TYPE -> i32
        case 36:
         printf("p36\n");
            Push_T_NT(stack,TOKEN_I32);
            Push_T_NT(stack,N_ARRAY_BRACE);
            Push_T_NT(stack,N_QUESTION);
            break;
        //TYPE -> f64
        case 37:
         printf("p37\n");
            Push_T_NT(stack,TOKEN_F64);
            Push_T_NT(stack,N_ARRAY_BRACE);
            Push_T_NT(stack,N_QUESTION);
            break;
        //TYPE -> u8
        case 38:
         printf("p38\n");
            Push_T_NT(stack,TOKEN_U8);
            Push_T_NT(stack,N_ARRAY_BRACE);
            Push_T_NT(stack,N_QUESTION);
            break;
        //TYPE -> void
        case 39:
         printf("p39\n");
            Push_T_NT(stack,TOKEN_VOID); // neviem ci to je samostatny token
            break;
        //LITERALS -> str_literal
        case 40:
         printf("p40\n");
             Push_T_NT(stack,TOKEN_STRING_LITERAL); // neviem ci to je samostatny token
            break;
        //LITERALS -> int_literal
        case 41:
         printf("41\n");
            Push_T_NT(stack,TOKEN_INTEGER_LITERAL);
            break;
        //LITERALS -> float_literal
        case 42:
         printf("p42\n");
           Push_T_NT(stack,TOKEN_FLOAT_LITERAL);
            break;
        //OPERATOR -> +
        case 43:
         printf("p43\n");
            Push_T_NT(stack,TOKEN_PLUS); //
            break;
        //OPERATOR -> -
        case 44://uz otocene 
         printf("p44\n");
            Push_T_NT(stack,TOKEN_MINUS); //
            break;
        //OPERATOR -> *
        case 45:
         printf("p45\n");
            Push_T_NT(stack,TOKEN_MULTIPLY); //
            break;
        //OPERATOR -> /
        case 46:
         printf("p46\n");
            Push_T_NT(stack,TOKEN_DIVIDE); 
            break;
        //OPERATOR -> ==
        case 47:
         printf("p47\n");
            Push_T_NT(stack,TOKEN_EQUAL); //
            break;
        //OPERATOR -> !=
        case 48:
         printf("p48\n");
            Push_T_NT(stack,TOKEN_NOT_EQUAL); //
            break;
        //OPERATOR -> >
        case 49:
         printf("p49\n");
            Push_T_NT(stack,TOKEN_GREATER); //
            break;
        //OPERATOR -> <
        case 50:
         printf("p50\n");       
            // Push_T_NT(stack,N_STATEMENT);
            Push_T_NT(stack,TOKEN_LESS); //
            break;
        //OPERATOR -> >=
        case 51:
         printf("p51\n");
            Push_T_NT(stack,TOKEN_GREATER_EQUAL); //
            break;
        //OPERATOR -> <=
        case 52:
         printf("p52\n");
            Push_T_NT(stack,TOKEN_LESS_EQUAL); //
            break;
        //SPLITER -> EXPRESION
        case 53:
         printf("p53\n");
            Push_T_NT(stack,N_EXPRESION); //
            break;
        //-> string_literal  NEXT_LITERAL
        case 54:
         printf("p54\n");
            // Push_T_NT(stack,TOKEN_RIGHT_BRACE); //
            break;
        //-> float_literal  NEXT_LITERAL
        case 55:
         printf("p55\n");
            Push_T_NT(stack,N_NEXT_LIT); //
            Push_T_NT(stack,TOKEN_FLOAT_LITERAL); //
            break;
        //-> int_float_literal  NEXT_LITERAL
        case 56:
         printf("p56\n");
            Push_T_NT(stack,N_NEXT_LIT); //
            Push_T_NT(stack,TOKEN_INTEGER_LITERAL); //
            break;
        //FN_CALL_TYPE -> token_id
        case 57:
         printf("p57\n");
            Push_T_NT(stack,TOKEN_IDENTIFIER); //
            break;
        //FN_CALL_TYPE -> float_literal
        case 58:
         printf("p58\n");
            Push_T_NT(stack,TOKEN_FLOAT_LITERAL); //
            break;
        //FN_CALL_TYPE -> int_literal
        case 59:
         printf("p59\n");
            Push_T_NT(stack,TOKEN_INTEGER_LITERAL); //
            break;
        //FN_CALL_TYPE -> str_literal
        case 60:
         printf("p60\n");
            Push_T_NT(stack,TOKEN_STRING_LITERAL); //
            break;
        //FN_CALL_COMA -> ''
        case 61:
         printf("p61\n");
            break;
        //FN_CALL_COMA -> ,
        case 62:
         printf("p62\n");
            Push_T_NT(stack,TOKEN_COMMA); //
            break;
        //TYPE-> null
        case 64:
         printf("p64\n");
            Push_T_NT(stack,TOKEN_NULL); //
            break;
        case 65:
         printf("p65\n");
            break;
        //OPERATOR_QUESTIONMARK-> ? 
        case 66:
         printf("p66\n");
            Push_T_NT(stack,TOKEN_Q_MARK); //
            break;
        //OPERATOR_QUESTIONMARK-> ''  
        case 67:
         printf("p67\n");
            break;
        //ARR_BRACE->[] 
        case 68:
         printf("p68\n");
            Push_T_NT(stack,TOKEN_ARRAY_BRACE); //
            break;
        //TYPE_PREFIX->  Q_MARK  ARR_BRACE  TYPE
        case 69:
         printf("p69\n");
            Push_T_NT(stack,N_TYPE); //
            Push_T_NT(stack,N_ARRAY_BRACE); //
            Push_T_NT(stack,N_QUESTION); //
            break;
        case 70:
            printf("70");
            Push_T_NT(stack,N_EXPRESION); 
            Push_T_NT(stack,TOKEN_RIGHT_PAREN); 
            Push_T_NT(stack,N_EXPRESION); 
            Push_T_NT(stack,TOKEN_LEFT_PAREN); 
        break;
        default:
        // printf("co tu robis picus\n");
            error_exit(ERR_SYNTAX, "zla syntax");
            break;
    }
}

/**
 * Analyzes the syntax of a given source file by processing tokens using a stack-based approach.
 *
 * This function repeatedly reads tokens from the given lexer context and compares them with the
 * non-terminal or terminal symbols on the top of the stack. It uses an LL parsing table to determine
 * the correct sequence of tokens and rules. If the syntax is valid, the function returns true;
 * otherwise, it exits with a syntax error.
 *
 * @param context A pointer to the LexerContext structure, which provides tokens from the source file.
 * @param stack A pointer to a TStack structure used to manage non-terminal and terminal symbols.
 * @return A boolean indicating whether the syntax is valid (true) or invalid (false).
 */
bool find_rule(LexerContext *context, TStack *stack) {
    Token token;
    stackSymbol *rule_stack;
    bool ok_syntax = true;

    Push_T_NT(stack, N_PROGRAM);
    
    // Načti první token před vstupem do smyčky
    token = get_token(context);
    print_test(token);

    while (!IsEmpty(stack)){ // asi to bude token eof ale neviem este 
        rule_stack = Top(stack);
        if (rule_stack->isTerm == 0){ //ak je na vrchole zasobnika Neterminal nahradi sa za dalsie pravidlo z LL tabulky
            //print_LLtable (token, rule_stack->value + __TOKENEND__ + 1);
            processValue(LL_TABLE[rule_stack->value][(int)token.type], stack);
            
        } else {
            if (rule_stack->value == (int)token.type){
                Pop(stack, free_stackSymbol);

                token = get_token(context);
                print_test(token);
            } else {
                ok_syntax = false;
                error_exit(ERR_SYNTAX, "zle poradie tokenov\n");
            }
        }
        i++;

    }
    PrintStack(stack);
    write_token_type(token);
    printf("\n");
    return ok_syntax;     
}

int main() {
    LexerContext context;
    initialize_context(&context, "ez.txt");

    TStack stack;
    stack_init(&stack);

    if (find_rule(&context, &stack)){
        printf("VALID\n");
    } else {
        printf("PICE\n");
    }

    // Closing lexer
    close_context(&context);
    
    // Closing stack
    stack_dtor(&stack, free_stackSymbol);



    return 0;
}
