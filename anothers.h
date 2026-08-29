#ifndef ANOTHERS_H
#define ANOTHERS_H

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "general.h"

//TODO: две структуры


int input     (values_s* consol_values);
int input_coef(double* const coef, const char err[]);

int solve                      (values_s* const consol_values);
numb_solutions line_roots      (const double coef_b,            const double coef_c);
numb_solutions partic_solutions(const double coef_c);
int line_eq                    (values_s* const consol_values);
double discriminant            (const values_s consol_values);
numb_solutions sq_roots        (const double discr);
int square_eq                  (values_s* const consol_values, const double discr);

bool isequal   (const double coef_1, const double coef_2);
bool clean_buf ();

int output    (const values_s consol_values);

#endif //ANOTHER_H
