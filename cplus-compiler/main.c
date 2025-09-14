#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "parser.h"
#include "compiler.h"

#define MAX_TOKENS 1024

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "TTS-CC: No input file provided.\n");
        fprintf(stderr, "Usage: %s <source.cplus>\n", argv[0]);
        return 1;
    }

    const char *input_path = argv[1];
    FILE *fp = fopen(input_path, "rb");
    if (!fp) {
        perror("TTS-CC: Failed to open input file");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *source = (char*)malloc(size + 1);
    if (!source) {
        fprintf(stderr, "TTS-CC: Out of memory.\n");
        fclose(fp);
        return 1;
    }

    fread(source, 1, size, fp);
    source[size] = '\0';
    fclose(fp);

    Token tokens[MAX_TOKENS];
    int token_count = 0;

    Lexer L = { source, (int)size, 0, 1, 1 };
    while (1) {
        Token t = lex_next(&L);
        if (token_count < MAX_TOKENS) {
            tokens[token_count++] = t;
        }
        if (t.kind == TOK_EOF) break;
    }

    Parser P = { tokens, token_count, 0 };
    parse_program(&P);

    printf("TTS-CC C+ Compiler v0.1\n");
    printf("Loaded source file: %s (%ld bytes)\n", input_path, size);
    printf("---- Source Preview ----\n%s\n------------------------\n", source);
    printf("---- Token Stream ----\n");
    for (int i = 0; i < token_count; i++) {
        Token t = tokens[i];
        printf("Token: %-10s text='%.*s'  line=%d, col=%d\n",
               tok_name(t.kind), t.length, t.lexeme, t.line, t.col);
    }

    if (out_cpp_emitter(argc, argv) == 0) {
        if (compile_to_exe(tokens, token_count, "out.cpp", "out.exe") == 0) {
            int run_rc = system(".\\out.exe");
            if (run_rc != 0) {
                fprintf(stderr, "TTS-CC: Running out.exe failed (rc=%d)\n", run_rc);
                return 1;
            }
            return 0;
        }
    }

    free(source);
    return 0;
}
