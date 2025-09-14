#ifndef COMPILER_H
#define COMPILER_H

#include "token.h"

// Returns 0 on success, non-zero on failure.
int compile_to_exe(Token* tokens, int count, const char* cpp_path, const char* exe_path);
int out_cpp_emitter(int argc, char** argv) {
    const char *input_path = argv[1];
    FILE *fp = fopen(input_path, "rb");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    char *source = (char*)malloc(size + 1);
    if (!fp) {
        perror("TTS-CC: Failed to open out.cpp for writing");
        return 1;
    }

    fprintf(fp, "#include <iostream>\n");
    fprintf(fp, "using namespace std;\n\n");
    fprintf(fp, "%s", source);

    fclose(fp);
    return 0;
}

#endif
