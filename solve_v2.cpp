#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define ALMOST_ZERO 0.00000001
#define ONE_NUMBER 1
#define NUM_HAND_TESTS 10
#define NUM_RANDOM_TESTS 1000

#define NO_COLOR "\033[0m"
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define PURPLE "\033[35m"

// Множество возможных количеств решений
/**
    \details Количество решений квадратного уравнения (от 0 до 2),
    бесконечное множество решений = 3,
    невозможное количество корней = -1,
    уравнение, которое невозможно решить = 4
*/

enum NUMB_SOLUTIONS
{
    NOT_NUM_SOLUTIONS = -1,
    ZERO_SOLUTIONS,
    ONE_SOLUTION,
    TWO_SOLUTIONS,
    INFINITY_SOLUTIONS,
    IMPOSSIBLE_EQUATION,
};

struct values_s
{
    NUMB_SOLUTIONS num_roots;
    double coef_a, coef_b, coef_c;                                  //TODO: две структуры
    double x1, x2;
};


int checking_answer();
int input          (values_s* consol_values);
int input_coef     (double* coef);
bool clean_buf     ();

int solve                    (values_s* consol_values);
NUMB_SOLUTIONS roots         (values_s consol_values,  double discr);
double discriminant          (values_s consol_values);
NUMB_SOLUTIONS checking_discr(double discr);
int square_eq                (values_s* consol_values, double discr);

bool isequal                 (double coef_1,          double coef_2);
NUMB_SOLUTIONS a_iszero      (double coef_b,           double coef_c);
NUMB_SOLUTIONS b_iszero      (double coef_c);

int output(values_s consol_values);

bool check_random_tests ();
int run_tests          ();
int run_one_test       (values_s test);
bool check_test        (values_s check_values, values_s test);
int random_tests       ();
bool check_random_roots(values_s rand_values);
double rand_doub       ();

//--------------------------------------------------------------------------------------------------------

int main()
{
    values_s consol_values = {.num_roots = IMPOSSIBLE_NUM_SOLUTIONS,
                              .coef_a = NAN, .coef_b = NAN, .coef_c = NAN,
                              .x1 = NAN, .x2 = NAN};

    if (checking_answer() == 'Y' && !check_random_tests())
    {
        return 0;
    }

    input(&consol_values);

    solve(&consol_values);

    output(consol_values);

    return 0;
}

//--------------------------------------------------------------------------------------------------------

//enter

int checking_answer()
{
    printf(YELLOW "Square equations solver\n");
    printf(YELLOW "Do you want to run tests before solving?" NO_COLOR);
    printf(GREEN "\nYes(Y)" RED "/NO(N)\n" NO_COLOR);

    int answer = 0;
    answer = toupper(getchar());
    while (answer != 'Y' && answer != 'N')
    {
        printf(PURPLE "Unknown command. Try again: " NO_COLOR);
        clean_buf();
        answer = toupper(getchar());
    }

    return answer;
}

/**
    \date Август 2026 года
    \brief Функция ввода коэффицентов
    \details Запрос на ввод коэффицентов квадратного уравнения
    \code
    int input(values_s* consol_values)
    {
        printf("\nSolving square equation\n");
        printf("Enter the coefficients a, b, c:");
        printf("\nEnter the coefficient before x^2: ");
        input_coef(&(consol_values -> coef_a));
        printf("\nEnter the coefficient before x: ");
        input_coef(&(consol_values -> coef_b));
        printf( "\nEnter the absolute term of equation: ");
        input_coef(&(consol_values -> coef_c));

        return 0;
    }
    \endcode
    \warning использовать с умом

*/

int input(values_s* consol_values)
{
    printf("\nSolving square equation\n");
    printf("Enter the coefficients a, b, c:");
    printf("\nEnter the coefficient before x^2: ");
    input_coef(&(consol_values -> coef_a));
    printf("\nEnter the coefficient before x: ");
    input_coef(&(consol_values -> coef_b));
    printf( "\nEnter the absolute term of equation: ");
    input_coef(&(consol_values -> coef_c));

    return 0;
}

/**
    \date Август 2026 года
    \brief Функция считывания коэффицентов квадратного уравнения
    \details Считывает коэффицент из командной строки,
    проверяет правильность ввода,
    при неправильном вводе выдаёт ошибку
    \param coef адрес коэффицента квадратного уравнения
    \warning Указатель не может быть пустым
*/

int input_coef(double* coef)
{
    assert(coef != NULL);

    printf("%s", err);
    bool flag = 1;
    while ((flag = scanf("%lg", coef)) != 1 || clean_buf() == false)
    {
        if (flag == 0)
        {
            clean_buf();
        }
        printf(RED "Input error. Try again! \n" NO_COLOR);
        printf("%s", err);
    }

    return 0;
}

//solving

int solve(values_s* consol_values)
 {
    assert(isfinite(consol_values -> coef_a));
    assert(isfinite(consol_values -> coef_b));
    assert(isfinite(consol_values -> coef_c));

    double discr = discriminant(*consol_values);
    (consol_values -> num_roots) = roots(*consol_values, discr);
    square_eq(consol_values, discr);

    return 0;
}



int line_eq(values_s* consol_values)
{
    assert(isfinite(discr));
    assert(isfinite(consol_values.coef_a));
    assert(isfinite(consol_values.coef_b));
    assert(isfinite(consol_values.coef_c));

    if (isequal(consol_values.coef_a, 0))
    {
        return a_iszero(consol_values.coef_b, consol_values.coef_c);
    }

    return checking_discr(discr);
}



double discriminant(values_s consol_values)
{
    assert(isfinite(consol_values.coef_a));
    assert(isfinite(consol_values.coef_b));
    assert(isfinite(consol_values.coef_c));

    double discr = consol_values.coef_b * consol_values.coef_b - 4 * consol_values.coef_a * consol_values.coef_c;
    return discr;
}



NUMB_SOLUTIONS checking_discr(double discr)
{
    assert(isfinite(discr));

    if (discr < 0)
    {
        return ZERO_SOLUTIONS;
    }
    else if (isequal(discr, 0))
    {
        return ONE_SOLUTION;
    }
    else
    {
        return TWO_SOLUTIONS;
    }
}



int square_eq(values_s* consol_values, double discr)
{
    assert(isfinite(consol_values -> coef_a));
    assert(isfinite(consol_values -> coef_b));
    assert(isfinite(discr));

    double sq_discr = sqrt(discr);
    (consol_values -> x1) = (0 - consol_values -> coef_b - sq_discr) / (2 * consol_values -> coef_a);
    (consol_values -> x2) = (0 - consol_values -> coef_b + sq_discr) / (2 * consol_values -> coef_a);

    return 0;
}

//help

bool isequal(double coef_1, double coef_2)
{
    if (fabs(coef_1 - coef_2) < ALMOST_ZERO || (isnan(coef_1) && isnan(coef_2)))
    {
        return true;
    }

    return false;
}



NUMB_SOLUTIONS a_iszero(double coef_b, double coef_c)
{
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));

    if (isequal(coef_b, 0))
    {
        return b_iszero(coef_c);
    }

    return ONE_SOLUTION;
}



NUMB_SOLUTIONS b_iszero(double coef_c)
{
    assert(isfinite(coef_c));

    return (isequal(coef_c, 0)) ? INFINITY_SOLUTIONS : IMPOSSIBLE_EQUATION;
}




//output

int output(values_s consol_values)
{
    assert(consol_values.num_roots != IMPOSSIBLE_NUM_SOLUTIONS);

    switch (consol_values.num_roots)
    {
        case ZERO_SOLUTIONS:  //no solutions
            printf("0 solutions\n");
            break;
        case ONE_SOLUTION:  //line equation - 1 root
            printf("Line equation: 1 solution\n");
            printf("x = %lg", consol_values.x1);
            break;
        case TWO_SOLUTIONS: //square equation - 2 roots
            printf("2 solutions:\n");
            printf("x1 = %lg\n", consol_values.x1);
            printf("x2 = %lg\n", consol_values.x2);
            break;
        case INFINITY_SOLUTIONS:
            printf("Infinite number of solutions\n");
            break;
        case IMPOSSIBLE_EQUATION:
            printf("Impossible to solve\n");
            break;
        case NOT_NUM_SOLUTIONS:
        default:
            printf("Error\n");
            break;
    }

    return 0;
}

//testing
bool go_tests()
{
    return checking_answer() == 'Y' && !check_all_tests();
}


bool check_random_tests()
{
    int num_tests = NUM_RANDOM_TESTS + NUM_HAND_TESTS;
    int success_tests = run_tests() + random_tests();

    char color = (success_tests == num_tests) ? printf(GREEN) : printf(RED);
    printf("%d/%d TESTS SUCCESFUL\n" NO_COLOR, success_tests, num_tests);
    {
        printf(RED "Sorry, but the solver doesn't work correctly. Please try again later.\n" NO_COLOR);
        return false;
    }

    return true;
}



int run_tests()
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
//TODO длину массива
    for (int num_test = 0; num_test  < NUM_HAND_TESTS; ++num_test)
    {
        test_success += run_one_test(tests[num_test]);
    }

    return test_success;
}



int run_one_test(values_s test)
{
    values_s check_values = {.num_roots = IMPOSSIBLE_NUM_SOLUTIONS,
                             .coef_a = test.coef_a, .coef_b = test.coef_b, .coef_c = test.coef_c,
                             .x1 = NAN, .x2 = NAN};
    solve(&check_values);
    if (!check_test(check_values, test))
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



bool check_test(values_s check_values, values_s test)
{
    return (check_values.num_roots == test.num_roots || (isequal(check_values.x1, test.x1) && isequal(check_values.x2, test.x2)));
}



int random_tests()
{
    int success = 0;
    for (int k = 0; k < NUM_RANDOM_TESTS; k++)
    {
        values_s rand_values = {.num_roots = IMPOSSIBLE_NUM_SOLUTIONS,
                                .coef_a = rand_doub(), .coef_b = rand_doub(), .coef_c = rand_doub(),
                                .x1 = NAN, .x2 = NAN};
        solve(&rand_values);
        if ((isnan(rand_values.x1) && isnan(rand_values.x2)) || (check_random_roots(rand_values)))
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


bool check_random_roots(values_s rand_values)
{
    if (isequal(rand_values.coef_a * rand_values.x1 * rand_values.x1 + rand_values.coef_b * rand_values.x1 + rand_values.coef_c, 0) &&
    isequal(rand_values.coef_a * rand_values.x2 * rand_values.x2 + rand_values.coef_b * rand_values.x2 + rand_values.coef_c, 0))
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
