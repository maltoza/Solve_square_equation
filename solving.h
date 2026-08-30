#ifndef SOLVING_H
#define SOVING_H

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "input.h"

int solve                      (const coefficients_s coefs, roots_s* const roots);
numb_solutions line_roots      (const double coef_b,               const double coef_c);
numb_solutions partic_solutions(const double coef_c);
int line_eq                    (const coefficients_s coefs, roots_s* const roots);
double discriminant            (const coefficients_s coefs);
numb_solutions sq_roots        (const double discr);
int square_eq                  (roots_s* const roots,       const double discr,          const coefficients_s coefs);

#endif //SOLVING_H
