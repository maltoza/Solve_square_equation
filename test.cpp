#include "test.h"


int checking_answer()
{
    printf(YELLOW "Square equations solver\n");
    printf(YELLOW "Do you want to run tests before solving?\n" NO_COLOR);
    printf(GREEN "Yes(Y)" RED "/NO(N)\n" NO_COLOR);
    int answer = 0;
    answer = toupper(getchar());
    while (answer != 'Y' && answer != 'N')
    {
        printf(RED "Unknown command. Try again!\n" NO_COLOR);
        printf(YELLOW "Do you want to run tests before solving?\n" NO_COLOR);
        printf(GREEN "Yes(Y)" RED "/NO(N)\n" NO_COLOR);
        clean_buf();
        answer = toupper(getchar());
    }

    return answer;
}

bool testing()
{
    return checking_answer() == 'Y' && !check_all_tests();
}



bool check_all_tests()
{
    int num_tests = NUM_RANDOM_TESTS + NUM_HAND_TESTS;
    int success_tests = hand_tests() + random_tests();
    if (num_tests != success_tests)
    {
        printf(RED "%d/%d TESTS SUCCESFUL\n" NO_COLOR, success_tests, num_tests);
        printf(RED "Sorry, but the solver doesn't work correctly. Please try again later.\n" NO_COLOR);
        return false;
    }
    else
    {
        printf(GREEN "%d/%d TESTS SUCCESFUL\n" NO_COLOR, success_tests, num_tests);
    }

    return true;
}



int hand_tests()
{
    int test_success = 0;
    values_s tests[NUM_HAND_TESTS] =
    {
        {.num_roots = ONE_SOLUTION,        .coef_a = 0, .coef_b = 2,  .coef_c = -2, .x1 = 1,   .x2 = 1  },
        {.num_roots = TWO_SOLUTIONS,       .coef_a = 2, .coef_b = 0,  .coef_c = -8, .x1 = -2,  .x2 = 2  },
        {.num_roots = IMPOSSIBLE_EQUATION, .coef_a = 0, .coef_b = 0,  .coef_c = 5,  .x1 = NAN, .x2 = NAN},
        {.num_roots = ONE_SOLUTION,        .coef_a = 2, .coef_b = 0,  .coef_c = 0,  .x1 = 0,   .x2 = 0  },
        {.num_roots = ONE_SOLUTION,        .coef_a = 0, .coef_b = 2,  .coef_c = 0,  .x1 = 0,   .x2 = 0  },
        {.num_roots = TWO_SOLUTIONS,       .coef_a = 1, .coef_b = -1, .coef_c = 0,  .x1 = 0,   .x2 = 1  },
        {.num_roots = INFINITY_SOLUTIONS,  .coef_a = 0, .coef_b = 0,  .coef_c = 0,  .x1 = NAN, .x2 = NAN},
        {.num_roots = TWO_SOLUTIONS,       .coef_a = 1, .coef_b = 3,  .coef_c = -2, .x1 = -2,  .x2 = -1 },
        {.num_roots = TWO_SOLUTIONS,       .coef_a = 2, .coef_b = 8,  .coef_c = 6,  .x1 = 1,   .x2 = 3  },
        {.num_roots = ONE_SOLUTION,        .coef_a = 0, .coef_b = 2,  .coef_c = 10, .x1 = -5,  .x2 = -5 }

    };
        //TODO длину массива sizeof()
    for (int num_test = 0; num_test  < NUM_HAND_TESTS; ++num_test)
    {
        test_success += run_hand_test(tests[num_test]);
    }

    return test_success;
}



int run_hand_test(const values_s test)
{
    values_s check_values = {.num_roots = INITIALIZ_VAL,
                             .coef_a = test.coef_a, .coef_b = test.coef_b, .coef_c = test.coef_c,
                             .x1 = NAN, .x2 = NAN};
    solve(&check_values);
    if (!check_hand_test(check_values, test))
    {
        printf(RED "                 Test FAILED\n" NO_COLOR
                   "           a = %lg, b = %lg, c = %lg\n"
                   "Expected: %d roots, x1 = %lg, x2 = %lg\n"
                   "Got:      %d roots, x1 = %lg, x2 = %lg\n",\
                         test.coef_a, test.coef_b, test.coef_c,\
                        test.num_roots, test.x1, test.x2,\
                    check_values.num_roots, check_values.x1, check_values.x2);

        return 0;
    }

    return 1;
}



bool check_hand_test(const values_s check_values, const values_s test)
{
    return (check_values.num_roots == test.num_roots || (isequal(check_values.x1, test.x1) && isequal(check_values.x2, test.x2)));
}



int random_tests()
{
    srand(time(NULL));
    values_s rand_values = {.num_roots = INITIALIZ_VAL,
                            .coef_a = NAN, .coef_b = NAN, .coef_c = NAN,
                            .x1 = NAN, .x2 = NAN};
    int success = 0;
    for (int k = 0; k < NUM_RANDOM_TESTS; k++)
    {
        rand_values.coef_a = rand();
        rand_values.coef_b = rand();
        rand_values.coef_c = rand();
        solve(&rand_values);
        if (check_random_test(rand_values))
        {
            success += 1;
        }
        else
        {
        printf(RED "              Test FAILED\n" NO_COLOR
                   "      a = %lg, b = %lg, c = %lg\n"
                   "Expected: %d roots, x1 = %lg, x2 = %lg\n"
                   "Got:      %d roots, x1 = %lg, x2 = %lg\n",\
                         rand_values.coef_a, rand_values.coef_b, rand_values.coef_c,\
                        rand_values.num_roots, rand_values.x1, rand_values.x2,\
                    rand_values.num_roots, rand_values.x1, rand_values.x2);
        }
    }

    return success;
}



bool check_random_test(const values_s rand_values)
{
    if ((rand_values.num_roots == ZERO_SOLUTIONS || rand_values.num_roots == IMPOSSIBLE_EQUATION
    || rand_values.num_roots == INFINITY_SOLUTIONS) && isnan(rand_values.x1) && isnan(rand_values.x2))
    {
        return true;
    }
    else if (rand_values.num_roots == ONE_SOLUTION && isnan(rand_values.x2) && check_sq_root(rand_values, rand_values.x1))
    {
        return true;
    }
    else if (rand_values.num_roots == TWO_SOLUTIONS && check_sq_root(rand_values, rand_values.x1)
    && check_sq_root(rand_values, rand_values.x2))
    {
        return true;
    }

    return false;
}



bool check_sq_root(const values_s rand_values, const double x)
{
    if (isequal(rand_values.coef_a * x * x + rand_values.coef_b * x + rand_values.coef_c, 0))
    {
        return true;
    }

    return false;
}



double rand_double()
{
    double x = RAND_MAX * (rand() / (RAND_MAX + 1.0));

    return x;
}
