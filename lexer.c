#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexer.h"
#include "expand_array.h"

void initialize_context(LexerContext *context, const char *filename) {
    context->filePointer = fopen(filename, "r");
    if (context->filePointer == NULL) {
        printf("Nemohu otevřít soubor.\n");
    }

    context->state = START;
}

void close_context(LexerContext *context) {
    if (context->filePointer != NULL) {
        fclose(context->filePointer);
    }
}

int peek(LexerContext *context) {
    // Získáme další znak z filePointeru bez posunutí ukazatele
    int ch = fgetc(context->filePointer);  // Čtení jednoho znaku

    if (ch != EOF) {
        ungetc(ch, context->filePointer);  // Vrácení znaku zpět do vstupního proudu
    }

    return ch;  // Vracíme znak, nebo EOF pokud jsme na konci souboru
}



TokenType get_double_op(const int ch) {
    if (ch == '>') return TOKEN_GREATER_EQUAL;
    if (ch == '<') return TOKEN_LESS_EQUAL;
    if (ch == '=') return TOKEN_EQUAL;
    if (ch == '!') return TOKEN_NOT_EQUAL;
    return TOKEN_UNKNOWN;
}

TokenType get_single_op(const int ch) {
    if (ch == '>') return TOKEN_GREATER;
    if (ch == '<') return TOKEN_LESS;
    if (ch == '=') return TOKEN_ASSIGNMENT;
    return TOKEN_UNKNOWN;
}


TokenType get_keyword_type(const char* str) {
    if (strcmp(str, "if") == 0) return TOKEN_IF;
    if (strcmp(str, "else") == 0) return TOKEN_ELSE;
    if (strcmp(str, "return") == 0) return TOKEN_RETURN;
    if (strcmp(str, "while") == 0) return TOKEN_WHILE;
    if (strcmp(str, "fn") == 0) return TOKEN_FN;
    if (strcmp(str, "const") == 0) return TOKEN_CONST; 
    if (strcmp(str, "pub") == 0) return TOKEN_PUB;
    if (strcmp(str, "void") == 0) return TOKEN_VOID;
    if (strcmp(str, "var") == 0) return TOKEN_VAR;
    if (strcmp(str, "i32") == 0) return TOKEN_I32; 
    if (strcmp(str, "f64") == 0) return TOKEN_F64; 
    if (strcmp(str, "null") == 0) return TOKEN_NULL; 
    if (strcmp(str, "u8") == 0) return TOKEN_U8; 
    return TOKEN_IDENTIFIER; // Pokud není klíčové slovo, je to identifikátor
}

bool is_hex_char(char ch) {
    return (ch >= '0' && ch <= '9') || 
           (ch >= 'a' && ch <= 'f') || 
           (ch >= 'A' && ch <= 'F');
}



Token get_token(LexerContext *context) {
    if (context->filePointer == NULL) {
        printf("Soubor není otevřen.\n");
        exit(1);
    }

    Token token;
    int ch = 0;

    while (1) {
        switch (context->state) {
            case START:
                while (isspace(ch = fgetc(context->filePointer))) {}

                if (isalpha(ch) || ch == '_') {
                    context->state = KEY_OR_IDENTIFIER;
                } else if (isdigit(ch)) {
                    context->state = NUMBER;
                } else if ((token.type = get_double_op(ch)) != TOKEN_UNKNOWN) {
                    if (peek(context) == '=') {
                        fgetc(context->filePointer); // Přeskočíme '='
                        context->state = START;
                        return token;
                    } else {
                        token.type = get_single_op(ch);
                        context->state = START;
                        return token;
                    }
                } else if (ch == '+') {
                    token.type = TOKEN_PLUS;
                    context->state = START;
                    return token;
                } else if (ch == '-') {
                    token.type = TOKEN_MINUS;
                    context->state = START;
                    return token;
                } else if (ch == '*') {
                    token.type = TOKEN_MULTIPLY;
                    context->state = START;
                    return token;
                } else if (ch == '/') {
                    if (peek(context) == '/') {
                        fgetc(context->filePointer); // Přeskočíme další '/'
                        context->state = COMMENT;
                    } else {
                        token.type = TOKEN_DIVIDE;
                        context->state = START;
                        return token;
                    }
                } else if (ch == '(') {
                    token.type = TOKEN_LEFT_PAREN;
                    context->state = START;
                    return token;
                } else if (ch == ')') {
                    token.type = TOKEN_RIGHT_PAREN;
                    context->state = START;
                    return token;
                } else if (ch == '{') {
                    token.type = TOKEN_LEFT_BRACE;
                    context->state = START;
                    return token;
                } else if (ch == '}') {
                    token.type = TOKEN_RIGHT_BRACE;
                    context->state = START;
                    return token;
                } else if (ch == '[') {
                    if (peek(context) == ']') {
                        fgetc(context->filePointer);
                        token.type = TOKEN_ARRAY_BRACE;
                    } else {
                       //ERROR LEXIKALNI CHYBA 
                    }
                    return token;
                    
                } else if (ch == ';') {
                    token.type = TOKEN_SEMICOLON;
                    context->state = START;
                    return token;
                } else if (ch == ',') {
                    token.type = TOKEN_COMMA;
                    context->state = START;
                    return token;
                } else if (ch == '|') {
                    token.type = TOKEN_PIPE;
                    context->state = START;
                    return token;
                } else if (ch == ':') {
                    token.type = TOKEN_COLON;
                    context->state = START;
                    return token;
                } else if (ch == '?') {
                    token.type = TOKEN_Q_MARK;
                    context->state = START;
                    return token;
                } else if (ch == '.') {
                    token.type = TOKEN_DOT;
                    context->state = START;
                    return token;  
                } else if (ch == '@') {
                    context->state = COMPTIME;
                } else if (ch == '"') {
                    context->state = STRING_LITERAL;
                } else if (ch == '\\' && peek(context) == '\\') {
                    context->state = MULTILINE_STRING;
                } else if (ch == EOF) {
                    token.type = TOKEN_EOF;
                    return token;
                } else {
                    token.type = TOKEN_UNKNOWN;
                    context->state = START;
                    return token;
                }
                break;

            case KEY_OR_IDENTIFIER: {
                token.value.arr.stringValue = init_array(&token.value.arr);
                add_char(&token.value.arr, ch);

                while (isalnum(ch = fgetc(context->filePointer)) || ch == '_') {
                    add_char(&token.value.arr, ch);
                }
                add_char(&token.value.arr, '\0');

                if (ch != EOF) {
                    ungetc(ch, context->filePointer);
                }

                if (strlen(token.value.arr.stringValue) == 1 && token.value.arr.stringValue[0] == '_') {
                    // Special type of variable, more info in 4.1.1 paragraph 2
                    token.type = TOKEN_UNDERSCORE;
                } else {
                    token.type = get_keyword_type(token.value.arr.stringValue);
                }
                
                context->state = START;
                return token;
            }

            case COMPTIME: {
                token.value.arr.stringValue = init_array(&token.value.arr);
                while (isalnum(ch = fgetc(context->filePointer)) || ch == '_') {
                    add_char(&token.value.arr, ch);
                }
                add_char(&token.value.arr, '\0');

                if (ch != EOF) {
                    ungetc(ch, context->filePointer);
                }

                if (ch != '(') {
                    context->state = START;
                    token.type = TOKEN_UNKNOWN;
                    return token;
                }
                
                token.type = TOKEN_COMPTIME;
                context->state = START;
                return token;
            }

            case NUMBER: {
                int is_float = 0;
                int is_exponent = 0;


                token.value.arr.stringValue = init_array(&token.value.arr);
                add_char(&token.value.arr, ch); // Uložení prvního čísla

                // Čteme zbytek čísel
                while (isdigit(ch = fgetc(context->filePointer)) || ch == '.' || ch == 'e' || ch == 'E') {
                    if (ch == '.') {
                        if (is_float || is_exponent) {
                            // Pokud již máme desetinnou tečku nebo exponent, je to chyba
                            token.type = TOKEN_UNKNOWN;
                            return token;
                        }
                        is_float = 1;
                    } else if (ch == 'e' || ch == 'E') {
                        if (is_exponent) {
                            // Více než jeden exponent, je to chyba
                            token.type = TOKEN_UNKNOWN;
                            return token;
                        }
                        is_exponent = 1;
                        is_float = 1; // Pokud je exponent, považujeme číslo za float
                        add_char(&token.value.arr, ch);

                        // Zpracování případného znaménka po 'e' nebo 'E'
                        ch = fgetc(context->filePointer);
                        if (ch == '+' || ch == '-') {
                            add_char(&token.value.arr, ch);
                            ch = fgetc(context->filePointer);
                        }

                        // Pokud po exponentu nejsou další číslice, je to chyba
                        if (!isdigit(ch)) {
                            token.type = TOKEN_UNKNOWN;
                            return token;
                        }
                    }

                    add_char(&token.value.arr, ch);
                }

                // Ukončení řetězce
                add_char(&token.value.arr, '\0');

                // Vrácení znaku zpět, pokud nebyl EOF
                if (ch != EOF) {
                    ungetc(ch, context->filePointer);
                }

                if (is_float) {
                    token.type = TOKEN_FLOAT_LITERAL;
                    token.value.floatValue = atof(token.value.arr.stringValue);
                } else {
                    token.type = TOKEN_INTEGER_LITERAL;
                    token.value.intValue = atoi(token.value.arr.stringValue);
                }
                context->state = START;
                return token;
            }

            case STRING_LITERAL: {
                token.value.arr.stringValue = init_array(&token.value.arr);
                    while ((ch = fgetc(context->filePointer)) != '"' && ch != EOF) {
                        if (ch == '\\') {
                            ch = fgetc(context->filePointer);
                            if (ch == 'x')  {
                                context->tokenBuffer[0] = '0';
                                context->tokenBuffer[1] = 'x';
                                if (peek(context) != EOF) context->tokenBuffer[2] = fgetc(context->filePointer);
                                if (peek(context) != EOF) context->tokenBuffer[3] = fgetc(context->filePointer);

                            // Kontrola, zda jsou oba znaky platné hexadecimální znaky
                            if (is_hex_char(context->tokenBuffer[2]) && is_hex_char(context->tokenBuffer[3])) {
                                context->tokenBuffer[4] = '\0'; // Ukončení řetězce
                                int value = (int)strtol(context->tokenBuffer, NULL, 16);
                                add_char(&token.value.arr, value);
                            } else {
                                printf("Error: Invalid hex value in string literal.\n");
                                exit(1);
                            }
                            } else if (ch == '"') {
                                add_char(&token.value.arr, '"');
                            } else if (ch == 'n') {
                                add_char(&token.value.arr, '\n');
                            } else if (ch == 'r') {
                                add_char(&token.value.arr, '\r');
                            } else if (ch == 't') {
                                add_char(&token.value.arr, '\t');
                            } else if (ch == '\\') {
                                add_char(&token.value.arr, '\\');
                            } else {
                                fprintf(stderr, "Error in escape sequence, \\%c doesn't exist.\n", ch);
                                exit(1);
                            }
                        } else {
                            add_char(&token.value.arr, ch);
                        }
                    }
                    add_char(&token.value.arr, '\0'); // Ukončení řetězce
                    token.type = TOKEN_STRING_LITERAL;
                    context->state = START;
                    return token;
                }

            case MULTILINE_STRING: {

                token.value.arr.stringValue = init_array(&token.value.arr);
                    fgetc(context->filePointer);
                    bool pass = true;
                    while (pass) {             
                        while ((ch = fgetc(context->filePointer)) != '\n' && ch != EOF) {
                            if (ch == '\\') {
                                ch = fgetc(context->filePointer);
                                if (ch == 'x')  {
                                    context->tokenBuffer[0] = '0';
                                    context->tokenBuffer[1] = 'x';
                                    if (peek(context) != EOF) context->tokenBuffer[2] = fgetc(context->filePointer);
                                    if (peek(context) != EOF) context->tokenBuffer[3] = fgetc(context->filePointer);

                                    // Kontrola, zda jsou oba znaky platné hexadecimální znaky
                                    if (is_hex_char(context->tokenBuffer[2]) && is_hex_char(context->tokenBuffer[3])) {
                                        context->tokenBuffer[4] = '\0'; // Ukončení řetězce
                                        int value = (int)strtol(context->tokenBuffer, NULL, 16);
                                        add_char(&token.value.arr, value);
                                    } else {
                                        printf("Error: Invalid hex value in string literal.\n");
                                        exit(1);
                                    }
                                } else if (ch == '"') {
                                    add_char(&token.value.arr, '"');
                                } else if (ch == 'n') {
                                    add_char(&token.value.arr, '\n');
                                } else if (ch == 'r') {
                                    add_char(&token.value.arr, '\r');
                                } else if (ch == 't') {
                                    add_char(&token.value.arr, '\t');
                                } else if (ch == '\\') {
                                    add_char(&token.value.arr, '\\');
                                } else {
                                    printf("Error in backslash in multiline string.\n");
                                    exit(1);
                                }
                            } else {
                                add_char(&token.value.arr, ch);
                            }
                        }

                        while (isspace(ch = fgetc(context->filePointer))) {}
                        if (ch == '\\' && peek(context) == '\\') {
                            fgetc(context->filePointer);
                            add_char(&token.value.arr, '\n');
                        } else {
                            ungetc(ch, context->filePointer);
                            pass = false;
                        }
                    }

                    add_char(&token.value.arr, '\0'); // Ukončení řetězce
                    token.type = TOKEN_STRING_LITERAL;
                    context->state = START;
                    return token;  
                }

            case COMMENT:
                // Přeskočíme vše do konce řádku nebo bloku
                while ((ch = fgetc(context->filePointer)) != '\n' && ch != EOF) {}
                context->state = START;  // Pokračování od začátku po komentáři
                break;

            default:
                break;
                
        }
    }
}
