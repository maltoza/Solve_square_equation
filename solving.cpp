#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include "solving.hpp"


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
