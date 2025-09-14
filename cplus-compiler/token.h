#pragma once

typedef enum {
    TOK_EOF = 0,
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_STRING,
    TOK_FUNC,
    TOK_RETURN,
    TOK_LET,
    TOK_PRINTLINE,
    TOK_LPAREN, TOK_RPAREN,
    TOK_LBRACE, TOK_RBRACE,
    TOK_SEMI, TOK_ASSIGN, TOK_UNKNOWN, TOK_INCLUDE

} TokKind;

typedef struct {
    TokKind kind;
    const char* lexeme;
    int length;
    int line;
    int col;
} Token;


typedef struct {
    const char* src;
    int length;
    int index;
    int line;
    int col;
} Lexer;

Token lex_next(Lexer* L);

static const char* tok_name(TokKind kind) {
    switch (kind) {
        case TOK_EOF: return "EOF";
        case TOK_IDENTIFIER: return "IDENTIFIER";
        case TOK_NUMBER: return "NUMBER";
        case TOK_STRING: return "STRING";
        case TOK_FUNC: return "FUNC";
        case TOK_RETURN: return "RETURN";
        case TOK_LET: return "LET";
        case TOK_PRINTLINE: return "PRINTLINE";
        case TOK_LPAREN: return "LPAREN";
        case TOK_RPAREN: return "RPAREN";
        case TOK_LBRACE: return "LBRACE";
        case TOK_RBRACE: return "RBRACE";
        case TOK_SEMI: return "SEMI";
        case TOK_ASSIGN: return "ASSIGN";
        case TOK_UNKNOWN: return "UNKNOWN";
        case TOK_INCLUDE: return "INCLUDE";
        default: return "???";
    }
}

