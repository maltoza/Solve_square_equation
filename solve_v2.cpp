#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
//TODO: сохранять на гитхаб как можно чаще
#define ALMOST_ZERO 0.00000001
#define ONE_NUMBER 1
#define NUMB_TESTS 10
//TODO: цвета через define попробовать свернуть в параметр(тип функции))  Color_text (str color)
#define NO_COLOR "\033[0m"
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define PURPLE "\033[35m"
//TODO: почитать про const
enum NUMB_SOLUTIONS
{
    IMPOSSIBLE_NUM_SOLUTIONS = -1,
    ZERO_SOLUTIONS,
    ONE_SOLUTION,
    TWO_SOLUTIONS,
    INFINITY_SOLUTIONS,
    IMPOSSIBLE_EQUATION,
};

struct values //TODO: переделать название чтобы было понятно, что это тип данных например добваить _t _s _k в конце
{
    NUMB_SOLUTIONS num_roots;
    double coef_a, coef_b, coef_c;
    double x1, x2;
};

//TODO: прочитать про const, подумать куда их можно применить

int checking_answer          (values* consol_values);
int input                    (values* consol_values);                                                // Enter coefficients
int input_coef               (double* coef);
bool clean_buf();
//TODO: отделить вспомогательные блоки
int solve                    (values* consol_values);
NUMB_SOLUTIONS roots         (values consol_values,  double discr);                                  //number of roots
bool isequal                  (double coef_1,         double coef_2);                                 //checking if the number is zero
NUMB_SOLUTIONS a_iszero      (double coef_b,         double coef_c);                                 //if a is zero
NUMB_SOLUTIONS b_iszero      (double coef_c);                                                        //if b is zero
double discriminant          (values consol_values);                                                 //counting discriminant
NUMB_SOLUTIONS checking_discr(double discr);
int line_eq                  (values* consol_values);                                                //solving line equation
int square_eq                (values* consol_values, double discr);                                  //solving square equation

int output                   (values consol_values);                                                 //output

bool check_work_comp();
int run_tests                ();
int run_one_test             (values test);
bool check_test              (values check_values, values test);

//--------------------------------------------------------------------------------------------------------
//TODO: сделать в main +- 4 команды: checking answer input solve output
//TODO: проверка сразу в main из 2 функций сверху типа yes no если нет то сразу свернуть программу
int main()
{
    values consol_values = {.num_roots = IMPOSSIBLE_NUM_SOLUTIONS, .coef_a = NAN, .coef_b = NAN, .coef_c = NAN, .x1 = NAN, .x2 = NAN};
    checking_answer(&consol_values);
    return 0;
}

//--------------------------------------------------------------------------------------------------------

//enter
//TODO: все таки постараться добавить assertы
//TODO сделать чтобы тесты запускались по выбору(вернуть test в switchcase)
int checking_answer(values* consol_values)
{
    printf( YELLOW "Do you want to solve square equations?" NO_COLOR);
    printf(GREEN "\nYes(Y)" RED "/NO(N)\n" NO_COLOR);

    int answer = 0;
    answer = toupper(getchar());
    while (answer != 'Y' && answer != 'N')
    {
        printf(PURPLE "Unknown command. Try again: " NO_COLOR);
        clean_buf();
        answer = toupper(getchar());
    }
    switch(answer)
    {
        case 'Y':
            if (!check_work_comp())
            {
                return 0;
            }

            input(consol_values);

            solve(consol_values);

            output(*consol_values);
            break;
        case 'N':
            printf(RED ">:(\n" NO_COLOR);
            break;
        default:
            break;
    }

    return 0;
}



int input(values* consol_values)
{
    printf("Solving square equation\n");
    printf("Enter the coefficients a, b, c:");
    printf("\nEnter the coefficient before x^2: ");
    input_coef(&(consol_values -> coef_a));
    printf("\nEnter the coefficient before x: ");
    input_coef(&(consol_values -> coef_b));
    printf( "\nEnter the absolute term of equation: ");
    input_coef(&(consol_values -> coef_c));

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
        printf(PURPLE "Input error. Try again: " NO_COLOR);
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

int solve(values* consol_values)
 {
    double discr = discriminant(*consol_values);
    (consol_values -> num_roots) = roots(*consol_values, discr);
    switch (consol_values -> num_roots)
    {
        case ONE_SOLUTION:
            if (isequal(consol_values -> coef_a, 0))
            {
                line_eq(consol_values);
            }
            else
            {
                (consol_values) -> x1 = 0;
            }
            break;
        case TWO_SOLUTIONS:
            square_eq(consol_values, discr);
            break;
        case INFINITY_SOLUTIONS:
        case IMPOSSIBLE_EQUATION:
        case IMPOSSIBLE_NUM_SOLUTIONS:
        case ZERO_SOLUTIONS:
        default:
            break;
    }

    return 0;
 }



NUMB_SOLUTIONS roots(values consol_values, double discr)
{
    assert(isfinite(discr));

    if (isequal(consol_values.coef_a, 0))
    {
        return a_iszero(consol_values.coef_b, consol_values.coef_c);
    }

    return checking_discr(discr);
}



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



NUMB_SOLUTIONS b_iszero(double(coef_c))
{
    assert(isfinite(coef_c));

    return (isequal(coef_c, 0)) ? INFINITY_SOLUTIONS : IMPOSSIBLE_EQUATION;
}



double discriminant(values consol_values)
{
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



int line_eq(values* consol_values)
{
    assert(isfinite(consol_values -> coef_b));
    assert(isfinite(consol_values -> coef_c));

    (consol_values -> x1) = 0 - (consol_values -> coef_c)/(consol_values -> coef_b);

    return 0;
}



int square_eq(values* consol_values, double discr)
{
    assert(isfinite(consol_values -> coef_a));
    assert(isfinite(consol_values -> coef_b));
    assert(isfinite(discr));

    double sq_discr = sqrt(discr);
    (consol_values -> x1) = (- consol_values -> coef_b - sq_discr) / (2 * consol_values -> coef_a);
    (consol_values -> x2) = (- consol_values -> coef_b + sq_discr) / (2 * consol_values -> coef_a);

    return 0;
}

//output

int output(values consol_values)
{
    assert(isfinite(consol_values.num_roots));
    switch (consol_values.num_roots)
    {
        case ZERO_SOLUTIONS:  //no solutions
            printf("\n0 solutions");
            break;
        case ONE_SOLUTION:  //line equation - 1 root
            printf("\nLine equation: 1 solution\n");
            printf("x = %lg", consol_values.x1);
            break;
        case TWO_SOLUTIONS: //square equation - 2 roots
            printf("\n2 solutions:");
            printf("\nx1 = %lg", consol_values.x1);
            printf("\nx2 = %lg", consol_values.x2);
            break;
        case INFINITY_SOLUTIONS:
            printf("\nInfinite number of solutions");
            break;
        case IMPOSSIBLE_EQUATION:
            printf("\nImpossible to solve");
            break;
        case IMPOSSIBLE_NUM_SOLUTIONS:
        default:
            printf("\nError");
            break;
    }

    return 0;
}

//testing

bool check_work_comp()
{
    int success = run_tests();
    if (success != NUMB_TESTS)
    {
        printf(RED "\nSorry, but the solver doesn't work correctly. Please try again later.\n" NO_COLOR);
        return false;
    }

    return true;
}

//выводить количество тестов
int run_tests()
{
    int test_succes = 0;
    values test_1 = {.num_roots = ONE_SOLUTION,        .coef_a = 0, .coef_b = 2,  .coef_c = -2, .x1 = 1,   .x2 = NAN};
    test_succes += run_one_test(test_1);
    values test_2 = {.num_roots = TWO_SOLUTIONS,       .coef_a = 2, .coef_b = 0,  .coef_c = -8, .x1 = -2,  .x2 = 2};
    test_succes += run_one_test(test_2);
    values test_3 = {.num_roots = IMPOSSIBLE_EQUATION, .coef_a = 0, .coef_b = 0,  .coef_c = 5,  .x1 = NAN, .x2 = NAN};
    test_succes += run_one_test(test_3);
    values test_4 = {.num_roots = ONE_SOLUTION,        .coef_a = 2, .coef_b = 0,  .coef_c = 0,  .x1 = 0,   .x2 = NAN};
    test_succes += run_one_test(test_4);
    values test_5 = {.num_roots = ONE_SOLUTION,        .coef_a = 0, .coef_b = 2,  .coef_c = 0,  .x1 = 0,   .x2 = NAN};
    test_succes += run_one_test(test_5);
    values test_6 = {.num_roots = TWO_SOLUTIONS,       .coef_a = 1, .coef_b = -1, .coef_c = 0,  .x1 = 0,   .x2 = 1};
    test_succes += run_one_test(test_6);
    values test_7 = {.num_roots = INFINITY_SOLUTIONS,  .coef_a = 0, .coef_b = 0,  .coef_c = 0,  .x1 = NAN, .x2 = NAN};
    test_succes += run_one_test(test_7);
    values test_8 = {.num_roots = TWO_SOLUTIONS,       .coef_a = 1, .coef_b = 3,  .coef_c = -2, .x1 = -2,  .x2 = -1};
    test_succes += run_one_test(test_8);
    values test_9 = {.num_roots = TWO_SOLUTIONS,       .coef_a = 2, .coef_b = 8,  .coef_c = 6, .x1 = 1,    .x2 = 3};
    test_succes += run_one_test(test_9);
    values test_10 = {.num_roots = INFINITY_SOLUTIONS, .coef_a = 0, .coef_b = 2,  .coef_c = 10, .x1 = -5,  .x2 = NAN};
    test_succes += run_one_test(test_10);
    return test_succes;
}



int run_one_test(values test)
{
    values check_values = {.num_roots = IMPOSSIBLE_NUM_SOLUTIONS, .coef_a = test.coef_a, .coef_b = test.coef_b, .coef_c = test.coef_c, .x1 = NAN, .x2 = NAN};
    solve(&check_values);
    if (!check_test(check_values, test))
    {
        printf(RED "                   Test FAILED\n" NO_COLOR
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



bool check_test(values check_values, values test)
{
    return (check_values.num_roots == test.num_roots || (isequal(check_values.x1, test.x1) && isequal(check_values.x2, test.x2)));
}


//TODO: добавить функцию с рандомными тестами хз тыща




