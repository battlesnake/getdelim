#pragma once
#include <cstd/std.h>
#include <fixedstr/fixedstr.h>

bool fstr_getdelim(struct fstr *fs, char delim, FILE *in);
bool fstr_getline(struct fstr *fs, FILE *in);
