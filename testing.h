#ifndef TESTING_H
#define TESTING_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "solving.h"

bool testing          ();
int checking_answer   ();
bool check_all_tests  ();
int hand_tests        ();
int run_hand_test     (const coefficients_s test_coefs, const roots_s test_roots);
bool check_hand_test  (const roots_s check_roots,       const roots_s test_roots);
int random_tests      ();
bool run_random_test  (coefficients_s rand_coef,        roots_s* const rand_roots);
bool check_random_test(const coefficients_s rand_coefs, const roots_s rand_roots);
bool check_sq_root    (const coefficients_s rand_coefs, const double x);
double rand_double    ();

#endif //TESTING_H
