#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "solving.hpp"

int checking_answer();
bool testing          ();
bool check_all_tests  ();
int hand_tests        ();
int run_hand_test     (const values_s test);
bool check_hand_test  (const values_s check_values, const values_s test);
int random_tests      ();
bool check_random_test(const values_s rand_values);
bool check_sq_root    (const values_s rand_values, const double x);
double rand_double    ();
