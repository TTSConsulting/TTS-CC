#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "compiler.h"

// Helper: replace '/' with '\\' in-place
static void fix_slashes(char* path) {
    for (char* p = path; *p; p++) {
        if (*p == '/') *p = '\\';
    }
}

int out_cpp_emitter(int argc, char** argv) {
    const char *input_path = argv[1];
    FILE *fp = fopen("out.cpp", "w"); // write to out.cpp, not the input file
    if (!fp) {
        perror("TTS-CC: Failed to open out.cpp for writing");
        return 1;
    }

    fprintf(fp, "#include <iostream>\n");
    fprintf(fp, "using namespace std;\n\n");
    fprintf(fp, "int main() {\n");
    fprintf(fp, "    cout << \"Hello, World!\" << endl;\n");
    fprintf(fp, "    return 0;\n");
    fprintf(fp, "}\n");

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
        "C:\\MinGW\\bin\\g++.exe \"%s\" -o \"%s\"",
        cpp_fixed, exe_fixed);

    printf("Running: %s\n", cmd);
    int rc = system(cmd);
    if (rc != 0) {
     fprintf(stderr, "TTS-CC: g++ failed (rc=%d)\n", rc);
     return rc; // propagate failure
   }
   printf("Compilation successful: %s\n", exe_fixed);
   return 0;
}
