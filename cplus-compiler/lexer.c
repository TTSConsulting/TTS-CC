#include "token.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

static char peek(Lexer* L, int offset) {
    int i = L->index + offset;
    return (i < L->length) ? L->src[i] : '\0';
}

static char advance(Lexer* L) {
    char c = peek(L, 0);
    if (c == '\n') { L->line++; L->col = 1; }
    else { L->col++; }
    L->index++;
    return c;
}

static void skip_whitespace_and_comments(Lexer* L) {
    while (1) {
        char c = peek(L, 0);
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance(L);
        } else if (c == '/' && peek(L, 1) == '/') {
            while (peek(L, 0) && peek(L, 0) != '\n') advance(L);
        } else if (c == '/' && peek(L, 1) == '*') {
            advance(L); advance(L);
            while (peek(L, 0) && !(peek(L, 0) == '*' && peek(L, 1) == '/')) advance(L);
            if (peek(L, 0)) { advance(L); advance(L); }
        } else {
            break;
        }
    }
}

static Token make_token(Lexer* L, TokKind kind, const char* start, int len, int line, int col) {
    (void)L;  // ← tells the compiler “I know I’m not using this”
    Token t = { kind, start, len, line, col };
    return t;
}

static int is_ident_start(char c) {
    return isalpha((unsigned char)c) || c == '_';
}

static int is_ident_part(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

Token lex_next(Lexer* L) {
    skip_whitespace_and_comments(L);

    int line = L->line;
    int col = L->col;
    char c = peek(L, 0);

    if (!c) return make_token(L, TOK_EOF, L->src + L->index, 0, line, col);

    // Handle #include
    if (c == '#') {
        advance(L);  // consume '#'
        skip_whitespace_and_comments(L);

        const char* start = L->src + L->index;
        while (is_ident_part(peek(L, 0))) advance(L);
        int len = (int)(L->src + L->index - start);

        if (len == 7 && strncmp(start, "include", 7) == 0) {
            return make_token(L, TOK_INCLUDE, start, len, line, col);
        }

        return make_token(L, TOK_UNKNOWN, start, len, line, col);
    }

    // Identifiers and keywords
    if (is_ident_start(c)) {
        const char* start = L->src + L->index;
        advance(L);
        while (is_ident_part(peek(L, 0))) advance(L);
        int len = (int)(L->src + L->index - start);

        if (len == 4 && strncmp(start, "func", 4) == 0) return make_token(L, TOK_FUNC, start, len, line, col);
        if (len == 6 && strncmp(start, "return", 6) == 0) return make_token(L, TOK_RETURN, start, len, line, col);
        if (len == 3 && strncmp(start, "let", 3) == 0) return make_token(L, TOK_LET, start, len, line, col);
        if (len == 10 && strncmp(start, "printLine", 10) == 0)
         return make_token(L, TOK_PRINTLINE, start, len, line, col);


        return make_token(L, TOK_IDENTIFIER, start, len, line, col);
    }

    // Numbers
    if (isdigit((unsigned char)c)) {
        const char* start = L->src + L->index;
        advance(L);
        while (isdigit((unsigned char)peek(L, 0))) advance(L);
        int len = (int)(L->src + L->index - start);
        return make_token(L, TOK_NUMBER, start, len, line, col);
    }

    // Strings
    if (c == '"') {
        advance(L);
        const char* start = L->src + L->index;
        while (peek(L, 0) && peek(L, 0) != '"') {
            if (peek(L, 0) == '\\') advance(L);
            advance(L);
        }
        int len = (int)(L->src + L->index - start);
        if (peek(L, 0) == '"') advance(L);
        return make_token(L, TOK_STRING, start, len, line, col);
    }

    // Symbols
    c = advance(L);  // now update c before switch
    switch (c) {
        case '(': return make_token(L, TOK_LPAREN, L->src + L->index - 1, 1, line, col);
        case ')': return make_token(L, TOK_RPAREN, L->src + L->index - 1, 1, line, col);
        case '{': return make_token(L, TOK_LBRACE, L->src + L->index - 1, 1, line, col);
        case '}': return make_token(L, TOK_RBRACE, L->src + L->index - 1, 1, line, col);
        case ';': return make_token(L, TOK_SEMI, L->src + L->index - 1, 1, line, col);
        case '=': return make_token(L, TOK_ASSIGN, L->src + L->index - 1, 1, line, col);
        default: return make_token(L, TOK_UNKNOWN, L->src + L->index - 1, 1, line, col);
    }
}
