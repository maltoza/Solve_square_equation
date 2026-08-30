#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "general.h"
#include "helping.h"

int input_answer();
int input             (coefficients_s* const coefs);
int input_terminal_coef(double* const coef,          const char prompt[]);
bool input_file_coef  (double* const coef,          FILE * f_enter);

int output(const roots_s roots);

#endif //INPUT_H
