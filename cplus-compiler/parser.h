#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef struct {
    Token* tokens;
    int count;
    int index;
} Parser;

void parse_program(Parser* P);

#endif
