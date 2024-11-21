#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "expand_array.h"


#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define MAX_TOKEN_LENGTH 128
// Definition of token types
typedef enum {
    TOKEN_EOF,
    TOKEN_SEMICOLON,
    TOKEN_PUB,
    TOKEN_FN,
    TOKEN_IDENTIFIER,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_IF,
    TOKEN_WHILE,
    TOKEN_VAR,
    TOKEN_CONST,
    TOKEN_RETURN,
    randomShit,/////iba vyplna medzenu v ll kovi chybe
    TOKEN_ASSIGNMENT,
    TOKEN_DOT,
    TOKEN_COMPTIME, //pro @import
    TOKEN_PIPE,
    TOKEN_ELSE,
    TOKEN_I32,
    TOKEN_F64,
    TOKEN_U8,
    TOKEN_VOID,
    TOKEN_STRING_LITERAL,
    TOKEN_INTEGER_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_GREATER,
    TOKEN_LESS,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS_EQUAL,
  
    TOKEN_NULL,
    TOKEN_Q_MARK,
    TOKEN_ARRAY_BRACE,
    // Most likely variable
    TOKEN_UNDERSCORE,
    TOKEN_UNKNOWN,

    // End of tokens enum, don't change the position
    __TOKENEND__
} TokenType;


// Union to store token value
typedef union {
    DynamicCharArray arr; // For identifiers, keywords, strings
    int intValue;                       // For integer literals
    float floatValue;                   // For floating-point literals
} TokenValue;

// Structure representing a token
typedef struct {
    TokenType type;      // Type of the token
    TokenValue value;    // Value of the token
} Token;



typedef enum {
    // Starting point
    START,

    // State for identifiers and keywords (like 'if', 'else', variables, etc.)
    KEY_OR_IDENTIFIER,

    // For @import, etc
    COMPTIME,

    // State for processing numbers (integers, floating point numbers)
    NUMBER,

    // State for processing comments
    COMMENT,

    // State for processing string literals
    STRING_LITERAL,
    MULTILINE_STRING

} State;

// Structure for storing the lexer state
typedef struct {
    FILE *filePointer;                  // Pointer to the input file
    char tokenBuffer[MAX_TOKEN_LENGTH]; // Buffer for the current token
    int EOF_flag;
    State state;                        // State of the lexical analyzer (for multi-state reading)
} LexerContext;


void initialize_context(LexerContext *context, const char *filename);
void close_context(LexerContext *context);
Token get_token(LexerContext *context);
int peek(LexerContext *context); // Int bcs of EOF.

#endif // LEXER_H