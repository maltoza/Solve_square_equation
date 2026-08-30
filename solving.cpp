#include "solving.h"

/**
    \defgroup solving Решение
    \brief Собрание функций, отвечающих за решение уравнения
    @{
 */
/**
    \brief Решение уравнения
    \param coefs структура, содержащая коэффиценты квадратного уравнения
    \param roots указатель на структуру, содержащую количество корней и корни
    \details Проверка: если a = 0, уравнение линейное, line_roots() проверяет количество корней линейного уравнения,
    затем line_eq() вычисляет корнию. Если a != 0, уравнение квадратное, discriminant() вычисляет дискриминант,
    sq_roots() находит количество корней квадратного уравнения, square_eq() находит корни.
    \return 0 - успешное завершение работы
 */
int solve(const coefficients_s coefs, roots_s* const roots)
{
    assert(isfinite(coefs.coef_a));
    assert(isfinite(coefs.coef_b));
    assert(isfinite(coefs.coef_c));

    if (isequal(coefs.coef_a, 0))
    {
        roots -> num_roots = line_roots(coefs.coef_b, coefs.coef_c);
        line_eq(coefs, roots);
    }
    else
    {
        double discr = discriminant(coefs);
        (roots -> num_roots) = sq_roots(discr);
        square_eq(roots, discr, coefs);
    }

    return 0;
}


/**
    \brief Поиск количества корней линейного уравнения
    \param coef_b коэффицент при линейном члене уравнения
    \param coef_c коэффицент при свободном члене уравнения
    \details Если b = 0, возвращает partic_solutions() (либо бесконечное количество решений, либо невозможно решить уравнение).
    Если b != 0 возвращает 1 решение.
    \return ONE_SOLUTION - 1 решение
    \return partic_solutions()
 */
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


/**
    \brief Частные случаи линейного уравнения
    \param coef_c свободный член
    \details Если c = 0, возвращает бесконечное количество решений, в противном случае возвращает, что уравнение нельзя решить.
    \return INFINITY SOLUTIONS = 3 бесконечное множество решений
    \return IMPOSSIBLE_EQUATION = 4 уравнение невозможно решить
 */
numb_solutions partic_solutions(const double coef_c)
{
    assert(isfinite(coef_c));

    return (isequal(coef_c, 0)) ? INFINITY_SOLUTIONS : IMPOSSIBLE_EQUATION;
}


/**
    \brief Решение линейного уравнения
    \param coefs Структура, содержащая коэффиценты квадратного уравнения
    \param roots Указатель на структуру, содержащую количество корней и корни уравнения
    \details Если количество решений = 1, то roots -> x1 и roots -> x2 присваивается значение корня.
    В противном случае ничего не происходит и функция завершает работу.
    \return 0 - успешное завершение работы
 */
int line_eq(const coefficients_s coefs, roots_s* const roots)
{
    assert(isfinite(coefs.coef_a));
    assert(isfinite(coefs.coef_b));
    assert(isfinite(coefs.coef_c));

    if (roots -> num_roots == ONE_SOLUTION)
    {
        roots -> x1 = 0 - coefs.coef_c/coefs.coef_b;
        roots -> x2 = 0 - coefs.coef_c/coefs.coef_b;
    }

    return 0;
}


/**
    \brief Вычисление дискриминанта
    \param coefs Структура, содержащая коэффиценты квадратного уравнения
    \details Вычисляется дискриминант по формуле D = b*b - 4*a*c.
    \return discr - значение дискриминанта
 */
double discriminant(const coefficients_s coefs)
{
    assert(isfinite(coefs.coef_a));
    assert(isfinite(coefs.coef_b));
    assert(isfinite(coefs.coef_c));

    double discr = coefs.coef_b * coefs.coef_b - 4 * coefs.coef_a * coefs.coef_c;
    return discr;
}

/**
    \brief Подсчёт количества корней квадратного уравнения
    \param discr значение дискриминанта
    \details Проверяет, если D < 0, то 0 решений, если D = 0, то 1 решение, если D > 0, то два решения.
    \return ZERO_SOLUTIONS - 0 решений
    \return ONE_SOLUTION - 1 решение
    \return TWO_SOLUTIONS - 2 решения
 */

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


/**
    \brief Решение квадратного уравнения
    \param roots Структура, содержащая количество корней и корни уравнения
    \param discr Дискриминант
    \param coefs Структура, содержащая коэффиценты
    \details Подсчитывает корень из дискриминанта. Присваивает x1 значение меньшего корня и x2 значение большего корня.
    При совпадении корней им присваиваются одинаковые значения.
    \return 0 - успешное завершение работы

 */
int square_eq(roots_s* const roots, const double discr, const coefficients_s coefs)
{
    assert(isfinite(coefs.coef_a));
    assert(isfinite(coefs.coef_b));
    assert(isfinite(discr));

    double sq_discr = sqrt(discr);
    (roots -> x1) = (0 - coefs.coef_b - sq_discr) / (2 * coefs.coef_a);
    (roots -> x2) = (0 - coefs.coef_b + sq_discr) / (2 * coefs.coef_a);

    return 0;
}
/** @} */
