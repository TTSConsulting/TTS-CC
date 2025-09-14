#include "parser.h"
#include <stdio.h>

static Token peek(Parser* P) {
    return (P->index < P->count) ? P->tokens[P->index] : (Token){ TOK_EOF, NULL, 0, 0, 0 };
}

static Token advance(Parser* P) {
    return (P->index < P->count) ? P->tokens[P->index++] : (Token){ TOK_EOF, NULL, 0, 0, 0 };
}

static int match(Parser* P, TokKind kind) {
    if (peek(P).kind == kind) {
        advance(P);
        return 1;
    }
    return 0;
}

static void parse_stmt(Parser* P) {
    Token t = peek(P);
    if (t.kind == TOK_PRINTLINE) {
        advance(P);
        if (!match(P, TOK_LPAREN)) {
            printf("Expected '(' after printLine at line %d\n", t.line);
            return;
        }
        if (!match(P, TOK_STRING)) {
            printf("Expected string in printLine at line %d\n", t.line);
            return;
        }
        if (!match(P, TOK_RPAREN)) {
            printf("Expected ')' after string at line %d\n", t.line);
            return;
        }
        if (!match(P, TOK_SEMI)) {
            printf("Expected ';' after printLine at line %d\n", t.line);
            return;
        }
        printf("Parsed printLine statement\n");
        return;
    }

    if (t.kind == TOK_RETURN) {
        advance(P);
        if (!match(P, TOK_NUMBER)) {
            printf("Expected number after return at line %d\n", t.line);
            return;
        }
        if (!match(P, TOK_SEMI)) {
            printf("Expected ';' after return statement at line %d\n", t.line);
            return;
        }
        printf("Parsed return statement\n");
        return;
    }

    printf("Unknown statement at line %d\n", t.line);
    advance(P);
}

static void parse_func_decl(Parser* P) {
    if (!match(P, TOK_FUNC)) {
        printf("Expected 'func' at line %d\n", peek(P).line);
        return;
    }

    if (!match(P, TOK_IDENTIFIER)) {
        printf("Expected function name after 'func' at line %d\n", peek(P).line);
        return;
    }

    if (!match(P, TOK_LPAREN)) {
        printf("Expected '(' after function name at line %d\n", peek(P).line);
        return;
    }

    if (!match(P, TOK_RPAREN)) {
        printf("Expected ')' after '(' at line %d\n", peek(P).line);
        return;
    }

    if (!match(P, TOK_LBRACE)) {
        printf("Expected '{' to start function body at line %d\n", peek(P).line);
        return;
    }

    printf("Parsed function declaration\n");

    while (peek(P).kind != TOK_RBRACE && peek(P).kind != TOK_EOF) {
        parse_stmt(P);
    }

    if (!match(P, TOK_RBRACE)) {
        printf("Expected '}' to close function body at line %d\n", peek(P).line);
    }
}

void parse_program(Parser* P) {
    while (peek(P).kind != TOK_EOF) {
        if (peek(P).kind == TOK_FUNC) {
            parse_func_decl(P);
        } else {
            printf("Skipping unexpected token '%s' at line %d\n",
                   tok_name(peek(P).kind), peek(P).line);
            advance(P);
        }
    }
}

