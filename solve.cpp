#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define EPSILON 0.00000001 ///< Погрешность сравнения чисел типа double
#define NUM_HAND_TESTS 10 ///< Количество ручных тестов
#define NUM_RANDOM_TESTS 1000 ///< Количество рандомных тестов
#define MAX_LEN 50 ///< Максимальная длина строки

#define NO_COLOR "\033[0m" ///< Белый цвет
#define RED "\033[1;31m" ///< Красный цвет
#define YELLOW "\033[1;33m" ///< Жёлтый цвет
#define GREEN "\033[1;32m" ///< Зелёный цвет
#define PURPLE "\033[35m" ///< Фиолетовый цвет

/**
    \brief Множество решений квадратного уравнения
*/
enum numb_solutions
{
    INITIALIZ_VAL = -1, ///< Инициализирующее значение
    ZERO_SOLUTIONS, ///< 0 решений
    ONE_SOLUTION, ///< 1 решение
    TWO_SOLUTIONS, ///< 2 решения
    INFINITY_SOLUTIONS, ///< 3 Бесконечное количество решений
    IMPOSSIBLE_EQUATION, ///< 4 Уравнение невозможно решить
};



///\brief Структура, содержащая коэффиценты квадратного уравнения

struct coefficients_s
{
    double coef_a, ///< Коэффицент перед квадратным членом
    coef_b, ///< Коэффицент перед линейным членом
    coef_c; ///< Коэффицент перед свободным членом
};

///\brief Структура, содержащая количество корней и корни квадратного уравнения

struct roots_s
{
    numb_solutions num_roots; ///< Количество корней
    double x1, ///<Мменьший корень
    x2; ///< Больший корень
};



int input     (coefficients_s* const consol_coefs);
int input_coef(double* const coef, const char err[]);

int solve                      (const coefficients_s consol_coefs, roots_s* const consol_roots);
numb_solutions line_roots      (const double coef_b,               const double coef_c);
numb_solutions partic_solutions(const double coef_c);
int line_eq                    (const coefficients_s consol_coefs, roots_s* const consol_roots);
double discriminant            (const coefficients_s consol_coefs);
numb_solutions sq_roots        (const double discr);
int square_eq                  (roots_s* const consol_roots,       const double discr,          const coefficients_s consol_coefs);

bool isequal   (const double coef_1, const double coef_2);
bool clean_buf ();

int output(const roots_s consol_roots);

int checking_answer   ();
bool testing          ();
bool check_all_tests  ();
int hand_tests        ();
int run_hand_test     (const coefficients_s test_coefs, const roots_s test_roots);
bool check_hand_test  (const roots_s check_roots, const roots_s test_roots);
int random_tests      ();
bool check_random_test(const coefficients_s rand_coefs, const roots_s rand_roots);
bool check_sq_root    (const coefficients_s rand_coefs, const double x);
double rand_double    ();

//--------------------------------------------------------------------------------------------------------

int main()
{
    coefficients_s consol_coefs = {.coef_a = NAN, .coef_b = NAN, .coef_c = NAN};
    roots_s consol_roots = {.num_roots = INITIALIZ_VAL, .x1 = NAN, .x2 = NAN};

    if (testing())
    {
        return 0;
    }

    input(&consol_coefs);

    solve(consol_coefs, &consol_roots);

    output(consol_roots);

    return 0;
}

//--------------------------------------------------------------------------------------------------------

//enter

/**
    \brief Функция запроса ввода коэффицентов
    \details Запрос на ввод коэффицентов квадратного уравнения, вызов функции input_coef()
    \param consol_coefs Указатель на структуру коэффицентов consol_coef, созданную в main
    \return 0 - успешное завершение программы

*/

int input(coefficients_s* const consol_coefs)
{
    printf("Solving square equation\n");
    printf("Enter the coefficients a, b, c\n");

    input_coef(&(consol_coefs -> coef_a), "Enter the coefficient before x^2: ");
    input_coef(&(consol_coefs -> coef_b), "Enter the coefficient before x: ");
    input_coef(&(consol_coefs -> coef_c), "Enter the constant term: ");

    return 0;
}

/**
    \brief Функция считывания коэффицента квадратного уравнения
    \param coef адрес коэффицента квадратного уравнения
    \param err массив символов (строка), уточняющий, какой коэффицент нужно вводить
    \details Пишет в командную строку что нужно вводить, создаёт переменную flag,
    сигнализирующую о количестве правильно считанных слов.
    Запускает цикл, заканчивающийся, когда верно считано одно число или функция clean_buf() (очистка буфера) вернула true.
    В цикле если количество верно считанных чисел равно 0 (ни одного числа не считалось),
    очистка буфера запускается ещё раз, чтобы предотвратить ввод не численных значений.
    В цикле программа выводит ошибку ввода и предлагает ввести ещё раз нужный коэффицент.
    \return 0 - успешное завершение программы
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

/**
    \brief Решение уравнения
    \param consol_coefs структура, содержащая коэффиценты квадратного уравнения
    \param consol_roots указатель на структуру, содержащую количество корней и корни
    \details Проверка: если a = 0, уравнение линейное, line_roots() проверяет количество корней линейного уравнения,
    затем line_eq() вычисляет корнию. Если a != 0, уравнение квадратное, discriminant() вычисляет дискриминант,
    sq_roots() находит количество корней квадратного уравнения, square_eq() находит корни.
    \return 0 - успешное завершение работы

*/

int solve(const coefficients_s consol_coefs, roots_s* const consol_roots)
 {
    assert(isfinite(consol_coefs.coef_a));
    assert(isfinite(consol_coefs.coef_b));
    assert(isfinite(consol_coefs.coef_c));

    if (isequal(consol_coefs.coef_a, 0))
    {
        consol_roots -> num_roots = line_roots(consol_coefs.coef_b, consol_coefs.coef_c);
        line_eq(consol_coefs, consol_roots);
    }
    else
    {
        double discr = discriminant(consol_coefs);
        (consol_roots -> num_roots) = sq_roots(discr);
        square_eq(consol_roots, discr, consol_coefs);
    }

    return 0;
}

/**
    \brief Поиск количества корней линейного уравнения
    \param coef_b коэффицент при линейном член уравнения
    \param coef_c коэффицент при свободном члене уравнения
    \details Если b = 0, возвращает partic_solutions(), либо бесконечное количество решений, либо невозможно решить уравнение.
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
    \param consol_coefs Структура, содержащая коэффиценты квадратного уравнения
    \param consol_roots указатель на структуру, содержащую количество корней и корни уравнения
    \details Если количество решений = 1, consol_roots -> x1 присваивается значение корня.
    В противном случае ничего не происходит и функция завершает работу.
    \return 0 - успешное завершение работы
 */

int line_eq(const coefficients_s consol_coefs, roots_s* const consol_roots)
{
    assert(isfinite(consol_coefs.coef_a));
    assert(isfinite(consol_coefs.coef_b));
    assert(isfinite(consol_coefs.coef_c));

    if (consol_roots -> num_roots == ONE_SOLUTION)
    {
        consol_roots -> x1 = 0 - consol_coefs.coef_c/consol_coefs.coef_b;
    }

    return 0;
}

/**
    \brief Вычисление дискриминанта
    \param consol_coefs структура, содержащая коэффиценты квадратного уравнения
    \details Вычисляется дискриминант по формуле D = b*b - 4*a*c.
    \return discr - значение дискриминанта
 */

double discriminant(const coefficients_s consol_coefs)
{
    assert(isfinite(consol_coefs.coef_a));
    assert(isfinite(consol_coefs.coef_b));
    assert(isfinite(consol_coefs.coef_c));

    double discr = consol_coefs.coef_b * consol_coefs.coef_b - 4 * consol_coefs.coef_a * consol_coefs.coef_c;
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



int square_eq(roots_s* const consol_roots, const double discr, const coefficients_s consol_coefs)
{
    assert(isfinite(consol_coefs.coef_a));
    assert(isfinite(consol_coefs.coef_b));
    assert(isfinite(discr));

    double sq_discr = sqrt(discr);
    (consol_roots -> x1) = (0 - consol_coefs.coef_b - sq_discr) / (2 * consol_coefs.coef_a);
    (consol_roots -> x2) = (0 - consol_coefs.coef_b + sq_discr) / (2 * consol_coefs.coef_a);

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


/**
    \brief Очистка буфера
    \details Создаёт переменную flag со значнием true. Если в строке остались символы помимо '\\n',
    запускает цикл, считывающий по одному символу и удаляющий их из буфера, пока не дойдет до символа '\\n',
    затем удаляет его и заканчивает работу.
    Если цикл был запущен, значение flag меняется на false.
    \return flag = true, если буфер не очищался
    \return flag = false, если буфер очищался
*/
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

int output(const roots_s consol_roots)
{
    assert(consol_roots.num_roots != INITIALIZ_VAL);

    switch (consol_roots.num_roots)
    {
        case ZERO_SOLUTIONS:  //no solutions
            printf("0 solutions\n");
            break;
        case ONE_SOLUTION:  //line equation - 1 root
            printf("Line equation: 1 solution\n");
            printf("x = %lg", consol_roots.x1);
            break;
        case TWO_SOLUTIONS: //square equation - 2 roots
            printf("2 solutions:\n");
            printf("x1 = %lg\n", consol_roots.x1);
            printf("x2 = %lg\n", consol_roots.x2);
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

//testing

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
    coefficients_s test_coefs[NUM_HAND_TESTS] =
    {
        {.coef_a = 0, .coef_b = 2,  .coef_c = -2},
        {.coef_a = 2, .coef_b = 0,  .coef_c = -8},
        {.coef_a = 0, .coef_b = 0,  .coef_c = 5 },
        {.coef_a = 2, .coef_b = 0,  .coef_c = 0 },
        {.coef_a = 0, .coef_b = 2,  .coef_c = 0 },
        {.coef_a = 1, .coef_b = -1, .coef_c = 0 },
        {.coef_a = 0, .coef_b = 0,  .coef_c = 0 },
        {.coef_a = 1, .coef_b = 3,  .coef_c = -2},
        {.coef_a = 2, .coef_b = 8,  .coef_c = 6 },
        {.coef_a = 0, .coef_b = 2,  .coef_c = 10}

    };
    roots_s test_roots[NUM_HAND_TESTS] =
    {
        {.num_roots = ONE_SOLUTION,        .x1 = 1,   .x2 = 1  },
        {.num_roots = TWO_SOLUTIONS,       .x1 = -2,  .x2 = 2  },
        {.num_roots = IMPOSSIBLE_EQUATION, .x1 = NAN, .x2 = NAN},
        {.num_roots = ONE_SOLUTION,        .x1 = 0,   .x2 = 0  },
        {.num_roots = ONE_SOLUTION,        .x1 = 0,   .x2 = 0  },
        {.num_roots = TWO_SOLUTIONS,       .x1 = 0,   .x2 = 1  },
        {.num_roots = INFINITY_SOLUTIONS,  .x1 = NAN, .x2 = NAN},
        {.num_roots = TWO_SOLUTIONS,       .x1 = -2,  .x2 = -1 },
        {.num_roots = TWO_SOLUTIONS,       .x1 = 1,   .x2 = 3  },
        {.num_roots = ONE_SOLUTION,        .x1 = -5,  .x2 = -5 }
    };

    for (int num_test = 0; num_test  < NUM_HAND_TESTS; ++num_test)
    {
        test_success += run_hand_test(test_coefs[num_test], test_roots[num_test]);
    }

    return test_success;
}



int run_hand_test(const coefficients_s test_coefs, const roots_s test_roots)
{
    coefficients_s check_coefs = {.coef_a = test_coefs.coef_a, .coef_b = test_coefs.coef_b, .coef_c = test_coefs.coef_c};
    roots_s check_roots = {.num_roots = INITIALIZ_VAL, .x1 = NAN, .x2 = NAN};
    solve(check_coefs, &check_roots);
    if (!check_hand_test(check_roots, test_roots))
    {
        printf(RED "                 Test FAILED\n" NO_COLOR
                   "           a = %lg, b = %lg, c = %lg\n"
                   "Expected: %d roots, x1 = %lg, x2 = %lg\n"
                   "Got:      %d roots, x1 = %lg, x2 = %lg\n",\
                         test_coefs.coef_a, test_coefs.coef_b, test_coefs.coef_c,\
                        test_roots.num_roots, test_roots.x1, test_roots.x2,\
                    check_roots.num_roots, check_roots.x1, check_roots.x2);

        return 0;
    }

    return 1;
}



bool check_hand_test(const roots_s check_roots, const roots_s test_roots)
{
    return (check_roots.num_roots == test_roots.num_roots || (isequal(check_roots.x1, test_roots.x1) && isequal(check_roots.x2, test_roots.x2)));
}



int random_tests()
{
    srand(time(NULL));
    coefficients_s rand_coefs = {.coef_a = NAN, .coef_b = NAN, .coef_c = NAN};
    roots_s rand_roots = {.num_roots = INITIALIZ_VAL, .x1 = NAN, .x2 = NAN};
    int success = 0;
    for (int k = 0; k < NUM_RANDOM_TESTS; k++)
    {
        rand_coefs = {.coef_a = rand_double(), .coef_b = rand_double(), .coef_c = rand_double()};
        solve(rand_coefs, &rand_roots);
        if (check_random_test(rand_coefs, rand_roots))
        {
            success += 1;
        }
        else
        {
        printf(RED "              Test FAILED\n" NO_COLOR
                   "      a = %lg, b = %lg, c = %lg\n"
                   "Expected: %d roots, x1 = %lg, x2 = %lg\n"
                   "Got:      %d roots, x1 = %lg, x2 = %lg\n",\
                         rand_coefs.coef_a, rand_coefs.coef_b, rand_coefs.coef_c,\
                        rand_roots.num_roots, rand_roots.x1, rand_roots.x2,\
                    rand_roots.num_roots, rand_roots.x1, rand_roots.x2);
        }
    }

    return success;
}



bool check_random_test(const coefficients_s rand_coefs, const roots_s rand_roots)
{
    if ((rand_roots.num_roots == ZERO_SOLUTIONS || rand_roots.num_roots == IMPOSSIBLE_EQUATION
    || rand_roots.num_roots == INFINITY_SOLUTIONS) && isnan(rand_roots.x1) && isnan(rand_roots.x2))
    {
        return true;
    }
    else if (rand_roots.num_roots == ONE_SOLUTION && isnan(rand_roots.x2) && check_sq_root(rand_coefs, rand_roots.x1))
    {
        return true;
    }
    else if (rand_roots.num_roots == TWO_SOLUTIONS && check_sq_root(rand_coefs, rand_roots.x1)
    && check_sq_root(rand_coefs, rand_roots.x2))
    {
        return true;
    }

    return false;
}



bool check_sq_root(const coefficients_s rand_coefs, const double x)
{
    if (isequal(rand_coefs.coef_a * x * x + rand_coefs.coef_b * x + rand_coefs.coef_c, 0))
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
