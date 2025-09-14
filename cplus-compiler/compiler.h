#ifndef COMPILER_H
#define COMPILER_H

#include "token.h"

// Returns 0 on success, non-zero on failure.
int compile_to_exe(Token* tokens, int count, const char* cpp_path, const char* exe_path);
int out_cpp_emitter(int argc, char** argv);

#endif
