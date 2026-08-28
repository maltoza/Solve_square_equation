#include "testing.cpp"

//--------------------------------------------------------------------------------------------------------

int main()
{
    values_s consol_values = {.num_roots = INITIALIZ_VAL,
                              .coef_a = NAN, .coef_b = NAN, .coef_c = NAN,
                              .x1 = NAN, .x2 = NAN};

    if (testing())
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
    printf("Solving square equation\n");
    printf("Enter the coefficients a, b, c\n");

    input_coef(&(consol_values -> coef_a), "Enter the coefficient before x^2: ");
    input_coef(&(consol_values -> coef_b), "Enter the coefficient before x: ");
    input_coef(&(consol_values -> coef_c), "Enter the constant term: ");

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

int input_coef(double* const coef, const char err[])
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
        printf(RED "Input error. Try again!\n" NO_COLOR);
        printf("%s", err);
    }

    return 0;
}

//solving

int solve(values_s* const consol_values)
 {
    assert(isfinite(consol_values -> coef_a));
    assert(isfinite(consol_values -> coef_b));
    assert(isfinite(consol_values -> coef_c));

    if (isequal(consol_values -> coef_a, 0))
    {
        consol_values -> num_roots = line_roots(consol_values -> coef_b, consol_values -> coef_c);
        line_eq(consol_values);
    }
    else
    {
        double discr = discriminant(*consol_values);
        (consol_values -> num_roots) = sq_roots(discr);
        square_eq(consol_values, discr);
    }

    return 0;
}



numb_solutions line_roots(const double coef_b, const double coef_c)
{
    assert(isfinite(coef_b));
    assert(isfinite(coef_c));

    if (isequal(coef_b, 0))
    {
        return partic_solutions(coef_c);
    }

    return ONE_SOLUTION;
}



numb_solutions partic_solutions(const double coef_c)
{
    assert(isfinite(coef_c));

    return (isequal(coef_c, 0)) ? INFINITY_SOLUTIONS : IMPOSSIBLE_EQUATION;
}



int line_eq(values_s* const consol_values)
{
    assert(isfinite(consol_values -> coef_a));
    assert(isfinite(consol_values -> coef_b));
    assert(isfinite(consol_values -> coef_c));

    if (consol_values -> num_roots == ONE_SOLUTION)
    {
        consol_values -> x1 = 0 - consol_values -> coef_c/consol_values -> coef_b;
    }

    return 0;
}



double discriminant(const values_s consol_values)
{
    assert(isfinite(consol_values.coef_a));
    assert(isfinite(consol_values.coef_b));
    assert(isfinite(consol_values.coef_c));

    double discr = consol_values.coef_b * consol_values.coef_b - 4 * consol_values.coef_a * consol_values.coef_c;
    return discr;
}



numb_solutions sq_roots(const double discr)
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



int square_eq(values_s* const consol_values, const double discr)
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

bool isequal(const double coef_1, const double coef_2)
{
    if (fabs(coef_1 - coef_2) < EPSILON || (isnan(coef_1) && isnan(coef_2)))
    {
        return true;
    }

    return false;
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

//output

int output(const values_s consol_values)
{
    assert(consol_values.num_roots != INITIALIZ_VAL);

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
        case INITIALIZ_VAL:
        default:
            printf("Error\n");
            break;
    }

    return 0;
}
//тесты


