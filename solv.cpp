#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define ALMOST_ZERO 0.0000001
#define ONE_NUMBER 1

enum NUMB_SOLUTIONS
{
    IMPOSSIBLE_NUM_SOLUTIONS = -1,
    ZERO_SOLUTIONS,
    ONE_SOLUTION,
    TWO_SOLUTIONS,
    INFINITY_SOLUTIONS,
    IMPOSSIBLE_EQUATION,
};
//TODO: где еще можно применить структуры в программе
struct test_value
{
    NUMB_SOLUTIONS num_roots_ref;
    double coef_a, coef_b, coef_c;
    double x1_ref, x2_ref;
};


int checking_answer          (double* coef_a,            double* coef_b, double* coef_c, NUMB_SOLUTIONS* num_roots, double* x1, double* x2);
int input                    (double* coef_a,            double* coef_b, double* coef_c);                                                   // Enter coefficients
int input_coef               (double* coef);
bool clean_buf();

int solve                    (NUMB_SOLUTIONS* num_roots, double coef_a,  double coef_b,  double coef_c,             double* x1, double* x2);
NUMB_SOLUTIONS roots         (double coef_a,             double coef_b,  double coef_c,  double discr);                                     //number of roots
bool iszero                  (double coef_1,             double coef_2);                                                                    //checking if the number is zero
NUMB_SOLUTIONS a_iszero      (double coef_b,             double coef_c);                                                                    //if a is zero
NUMB_SOLUTIONS b_iszero      (double coef_c);                                                                                               //if b is zero
double discriminant          (double coef_a,             double coef_b,  double coef_c);                                                    //counting discriminant
NUMB_SOLUTIONS checking_discr(double discr);
int line_eq                  (double coef_b,             double coef_c,  double* x1);                                                       //solving line equation
int square_eq                (double coef_a,             double coef_b,  double* x1,     double* x2,                double discr);          //solving square equation

int output                   (NUMB_SOLUTIONS num_roots, double x1, double x2);                                                              //output

int run_tests                ();
int run_one_test             (test_value test);
bool check_test              (NUMB_SOLUTIONS num_roots, NUMB_SOLUTIONS num_roots_ref, double x1_check, double x1_ref, double x2_check, double x2_ref);

//--------------------------------------------------------------------------------------------------------

int main()
{
    double coef_a = NAN, coef_b = NAN, coef_c = NAN;
    double x1 = NAN, x2 = NAN;
    NUMB_SOLUTIONS num_roots = IMPOSSIBLE_NUM_SOLUTIONS;
    checking_answer(&coef_a, &coef_b, &coef_c, &num_roots, &x1, &x2);

    return 0;
}

//--------------------------------------------------------------------------------------------------------

//enter

int checking_answer(double* coef_a, double* coef_b, double* coef_c, NUMB_SOLUTIONS* num_roots, double* x1, double* x2)
{
    assert(coef_a != NULL);
    assert(coef_b != NULL);
    assert(coef_c != NULL);
    assert(num_roots != NULL);
    assert(x1 != NULL);
    assert(x2 != NULL);
    assert(x1 != x2);
    printf("Do you want to solve square equations?");
    printf("\nYes(Y)/NO(N)/Run tests(T)\n");

    int answer = 0;
    answer = toupper(getchar());
    while (answer != 'Y' && answer != 'N' && answer != 'T')
    {
        printf("Unknown command. Try again: ");
        clean_buf();
        answer = toupper(getchar());
    }
    switch(answer)
    {
        case 'Y':
            input(coef_a, coef_b, coef_c);

            solve(num_roots, *coef_a, *coef_b, *coef_c, x1, x2);

            output(*num_roots, *x1, *x2);
            break;
        case 'N':
            printf(":(\n");
            break;
        case 'T':
            run_tests();
            break;
        default:
            break;
    }

    return 0;
}



int input(double* coef_a, double* coef_b, double* coef_c)
{
    assert(coef_a != NULL);
    assert(coef_b != NULL);
    assert(coef_c != NULL);
    printf("Solving square equation\n");
    printf("Enter the coefficients a, b, c:");
    printf("\nEnter the coefficient before x^2: ");
    input_coef(coef_a);
    printf("\nEnter the coefficient before x: ");
    input_coef(coef_b);
    printf("\nEnter the absolute term of equation: ");
    input_coef(coef_c);

    return 0;
}



int input_coef(double* coef)
{
    assert(coef != NULL);
    bool flag = 1;
    while ((flag = scanf("%lg", coef)) != 1 || clean_buf() == false)
    {
        if (flag == 0)
        {
            clean_buf();
        }
        printf("Input error. Try again: \n");
    }

    return 0;
}



bool clean_buf()
{
    bool flag = true;

    while(getchar() != '\n')
    {
        flag = false;
    }

    return flag;
}

//solving

int solve(NUMB_SOLUTIONS* num_roots, double coef_a, double coef_b, double coef_c, double* x1, double* x2)
 {
    assert(num_roots != NULL);
    assert(isfinite(coef_a));
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));
    assert(x1 != NULL);
    assert(x2 != NULL);
    assert(x1 != x2);
    double discr = discriminant(coef_a, coef_b, coef_c);
    *num_roots = roots(coef_a, coef_b, coef_c, discr);
    switch (*num_roots)
    {//TODO: система контроля версий и реализрвать идею со структурой и поменять в первонач структуре назв переменных
        case ONE_SOLUTION:
            if (iszero(coef_a, 0))
            {
                line_eq(coef_b, coef_c, x1);
            }
            else
            {
                *x1 = 0;
            }
            break;
        case TWO_SOLUTIONS:
            square_eq(coef_a, coef_b, x1, x2, discr);
            break;
        default:
            break;
    }

    return 0;
 }



NUMB_SOLUTIONS roots(double coef_a, double coef_b, double coef_c, double discr)
{
    assert(isfinite(coef_a));
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));
    assert(isfinite(discr));
    if (iszero(coef_a, 0))
    {
        return a_iszero(coef_b, coef_c);
    }

    return checking_discr(discr);
}



bool iszero(double coef_1, double coef_2)
{
    assert(isfinite(coef_1));
    assert(isfinite(coef_2));
    if (fabs(coef_1 - coef_2) < ALMOST_ZERO)
    {
        return true;
    }

    return false;
}



NUMB_SOLUTIONS a_iszero(double coef_b, double coef_c)
{
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));
    if (iszero(coef_b, 0))
    {
        return b_iszero(coef_c);
    }

    return ONE_SOLUTION;
}



NUMB_SOLUTIONS b_iszero(double(coef_c))
{
    assert(isfinite(coef_c));
    return (iszero(coef_c, 0)) ? INFINITY_SOLUTIONS : IMPOSSIBLE_EQUATION;
}



double discriminant(double coef_a, double coef_b, double coef_c)
{
    assert(isfinite(coef_a));
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));
    double discr = coef_b * coef_b - 4 * coef_a * coef_c;
    return discr;
}



NUMB_SOLUTIONS checking_discr(double discr)
{
    assert(isfinite(discr));
    if (discr < 0)
    {
    return ZERO_SOLUTIONS;
    }
    else if (iszero(discr, 0))
    {
    return ONE_SOLUTION;
    }
    else
    {
    return TWO_SOLUTIONS;
    }
}



int line_eq(double coef_b, double coef_c, double* x1)
{
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));
    assert(x1 != NULL);
    *x1 = 0 - coef_c/coef_b;

    return 0;
}



int square_eq(double coef_a, double coef_b, double* x1, double* x2, double discr)
{
    assert(isfinite(coef_a));
    assert(isfinite(coef_b));
    assert(x1 != NULL);
    assert(x2 != NULL);
    assert(x1 != x2);
    assert(isfinite(discr));
    double sq_discr = sqrt(discr);
    *x1 = (- coef_b - sq_discr) / (2 * coef_a);
    *x2 = (- coef_b + sq_discr) / (2 * coef_a);

    return 0;
}

//output

int output(NUMB_SOLUTIONS num_roots, double x1, double x2)
{
    assert(isfinite(num_roots));
    switch (num_roots)
    {
        case ZERO_SOLUTIONS:  //no solutions
            printf("\n0 solutions");
            break;
        case ONE_SOLUTION:  //line equation - 1 root
            printf("\nLine equation: 1 solution\n");
            printf("x = %lg", x1);
            break;
        case TWO_SOLUTIONS: //square equation - 2 roots
            printf("\n2 solutions:");
            printf("\nx1 = %lg", x1);
            printf("\nx2 = %lg", x2);
            break;
        case INFINITY_SOLUTIONS:
            printf("\nInfinite number of solutions");
            break;
        case IMPOSSIBLE_EQUATION:
            printf("\nImpossible to solve");
            break;
        default:
            printf("\nError");
            break;
    }

    return 0;
}

//testing

int run_tests()
{
    int test_succes = 0;
    test_value test_1 = {.num_roots_ref = ONE_SOLUTION,        .coef_a = 0, .coef_b = 2,  .coef_c = -2, .x1_ref = 1,   .x2_ref = NAN};
    test_succes += run_one_test(test_1);
    test_value test_2 = {.num_roots_ref = TWO_SOLUTIONS,       .coef_a = 2, .coef_b = 0,  .coef_c = -8, .x1_ref = -2,  .x2_ref = 2};
    test_succes += run_one_test(test_2);
    test_value test_3 = {.num_roots_ref = IMPOSSIBLE_EQUATION, .coef_a = 0, .coef_b = 0,  .coef_c = 5,  .x1_ref = NAN, .x2_ref = NAN};
    test_succes += run_one_test(test_3);
    test_value test_4 = {.num_roots_ref = ONE_SOLUTION,        .coef_a = 2, .coef_b = 0,  .coef_c = 0,  .x1_ref = 0,   .x2_ref = NAN};
    test_succes += run_one_test(test_4);
    test_value test_5 = {.num_roots_ref = ONE_SOLUTION,        .coef_a = 0, .coef_b = 2,  .coef_c = 0,  .x1_ref = 0,   .x2_ref = NAN};
    test_succes += run_one_test(test_5);
    test_value test_6 = {.num_roots_ref = TWO_SOLUTIONS,       .coef_a = 1, .coef_b = -1, .coef_c = 0,  .x1_ref = 0,   .x2_ref = 1};
    test_succes += run_one_test(test_6);
    test_value test_7 = {.num_roots_ref = INFINITY_SOLUTIONS,  .coef_a = 0, .coef_b = 0,  .coef_c = 0,  .x1_ref = NAN, .x2_ref = NAN};
    test_succes += run_one_test(test_7);
    test_value test_8 = {.num_roots_ref = TWO_SOLUTIONS,       .coef_a = 1, .coef_b = 3,  .coef_c = -2, .x1_ref = -2,  .x2_ref = -1};
    test_succes += run_one_test(test_8);
    printf("%d/8 tests succesful", test_succes);
    return 0;
}



int run_one_test(test_value test)
{
    double x1_check = NAN, x2_check = NAN;
    NUMB_SOLUTIONS num_roots = IMPOSSIBLE_NUM_SOLUTIONS;
    solve(&num_roots, test.coef_a, test.coef_b, test.coef_c, &x1_check, &x2_check);
    if (!check_test(num_roots, test.num_roots_ref, x1_check, test.x1_ref, x2_check, test.x2_ref))
    {
        printf("                   Test FAILED\n"
               "           a = %lg, b = %lg, c = %lg\n"
               "Expected: %d roots, x1 = %lg, x2 = %lg\n"
               "Got:      %d roots, x1 = %lg, x2 = %lg\n",\
                         test.coef_a, test.coef_b, test.coef_c,\
                    test.num_roots_ref, test.x1_ref, test.x2_ref,\
                             num_roots, x1_check, x2_check);

        return 0;
    }

    return 1;
}



bool check_test(NUMB_SOLUTIONS num_roots, NUMB_SOLUTIONS num_roots_ref, double x1_check, double x1_ref, double x2_check, double x2_ref)
{
    assert(isfinite(num_roots));
    assert(isfinite(num_roots_ref));
    return ((num_roots == num_roots_ref || (isnan(x1_check) && isnan(x1_ref) && isnan(x2_check) && isnan(x2_ref)) )\
     || (iszero(x1_check, x1_ref) && isnan(x2_check) && isnan(x2_ref)) || (iszero(x1_check, x1_ref) || iszero(x2_check, x2_ref)));
}
