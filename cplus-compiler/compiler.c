#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "compiler.h"

// Tiny helper to compare a token's text to a C string
static int tok_eq(Token t, const char* s) {
    size_t n = strlen(s);
    return (size_t)t.length == n && strncmp(t.lexeme, s, n) == 0;
}

// Helper: replace '/' with '\\' in-place
static void fix_slashes(char* path) {
    for (char* p = path; *p; p++) {
        if (*p == '/') *p = '\\';
    }
}

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

int compile_to_exe(Token* tokens, int count, const char* cpp_path, const char* exe_path) {
    char cpp_fixed[260];
    char exe_fixed[260];

    snprintf(cpp_fixed, sizeof(cpp_fixed), "%s", cpp_path);
    snprintf(exe_fixed, sizeof(exe_fixed), "%s", exe_path);

    fix_slashes(cpp_fixed);
    fix_slashes(exe_fixed);

    char cmd[1024];
    snprintf(cmd, sizeof(cmd),
    "C:\\MinGW\\bin\\g++.exe %s -o \"%s\"",
    cpp_fixed, exe_fixed);
    printf("Running: %s\n", cmd);
    int rc = system(cmd);



    FILE *pipe = _popen(cmd, "r");
if (!pipe) {
    perror("TTS-CC: Failed to run g++");
    return 1;
}

char buffer[256];
while (fgets(buffer, sizeof(buffer), pipe)) {
    fputs(buffer, stdout);
}

if (rc == 0) {
    printf("Compilation successful: %s\n", exe_fixed);
} else {
    fprintf(stderr, "TTS-CC: g++ failed (rc=%d)\n", rc);
    return rc; // stop here, don’t try to run the exe
}




    printf("Compilation successful: %s\n", exe_fixed);
    return 0;
}
