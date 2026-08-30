/**
    \file
    \brief Документация программы solve.cpp
 */


#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h> //TODO заголовки в html, считывание из файла, мб защиту ввода, поделить программу на файлы

#define EPSILON 0.00000001 ///< Погрешность сравнения чисел типа double
#define NUM_HAND_TESTS 10 ///< Количество ручных тестов
#define NUM_RANDOM_TESTS 1000 ///< Количество рандомных тестов
#define MAX_LEN 50 ///< Максимальная длина строки

#define NO_COLOR "\033[0m" ///< Отмена цвета
#define RED "\033[1;31m" ///< Красный цвет
#define YELLOW "\033[1;33m" ///< Жёлтый цвет
#define GREEN "\033[1;32m" ///< Зелёный цвет
#define PURPLE "\033[35m" ///< Фиолетовый цвет


///\brief Множество решений квадратного уравнения
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
    double coef_a; ///< Коэффицент перед квадратным членом
    double coef_b; ///< Коэффицент перед линейным членом
    double coef_c; ///< Коэффицент перед свободным членом
};


///\brief Структура, содержащая количество корней и корни квадратного уравнения
struct roots_s
{
    numb_solutions num_roots; ///< Количество корней
    double x1; ///< Меньший корень
    double x2; ///< Больший корень
};


int input     (coefficients_s* const consol_coefs);
int input_coef(double* const coef,                 const char prompt[]);

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

/**
    \defgroup enter Ввод
    \brief Собрание функций, отвечающих за ввод коэффицентов
    @{
 */
/**
    \brief Функция запроса ввода коэффицентов
    \param consol_coefs Указатель на структуру коэффицентов consol_coef, созданную в main()
    \details Запрос на ввод коэффицентов квадратного уравнения, вызов функции input_coef()
    \return 0 - успешное завершение работы функции

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
    \param coef Адрес коэффицента квадратного уравнения
    \param prompt Массив символов (строка), уточняющий, какой коэффицент нужно вводить
    \details Пишет в командную строку что нужно вводить, создаёт переменную flag,
    сигнализирующую о количестве правильно считанных слов.
    Запускает цикл, заканчивающийся, когда верно считано одно число и функция clean_buf() (очистка буфера) вернула true.
    В цикле, если количество верно считанных чисел равно 0 (ни одного числа не считалось),
    очистка буфера запускается ещё раз, т.к. она не сработала в условии цикла из-за того,
    что первый аргумент логического "или" (||) вернул true.
    В цикле программа выводит ошибку ввода и предлагает ввести ещё раз нужный коэффицент.
    \return 0 - успешное завершение программы
 */
int input_coef(double* const coef, const char prompt[])
{
    assert(coef != NULL);

    printf("%s", prompt);
    int flag = 1;
    while ((flag = scanf("%lg", coef)) != 1 || clean_buf() == false)
    {
        if (flag == 0)
        {
            clean_buf();
        }
        printf(RED "Input error. Try again!\n" NO_COLOR);
        printf("%s", prompt);
    }

    return 0;
}
/** @} */


/**
    \defgroup solving Решение
    \brief Собрание функций, отвечающих за решение уравнения
    @{
 */
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
    \param consol_coefs Структура, содержащая коэффиценты квадратного уравнения
    \param consol_roots Указатель на структуру, содержащую количество корней и корни уравнения
    \details Если количество решений = 1, то consol_roots -> x1 и consol_roots -> x2 присваивается значение корня.
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
        consol_roots -> x2 = 0 - consol_coefs.coef_c/consol_coefs.coef_b;
    }

    return 0;
}


/**
    \brief Вычисление дискриминанта
    \param consol_coefs Структура, содержащая коэффиценты квадратного уравнения
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


/**
    \brief Решение квадратного уравнения
    \param consol_roots Структура, содержащая количество корней и корни уравнения
    \param discr Дискриминант
    \param consol_coefs Структура, содержащая коэффиценты
    \details Подсчитывает корень из дискриминанта. Присваивает x1 значение меньшего корня и x2 значение большего корня.
    При совпадении корней им присваиваются одинаковые значения.
    \return 0 - успешное завершение работы

 */
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
/** @} */


/**
    \defgroup helping Помощь
    \brief Собрание вспомогательных функций
    @{
 */
/**
    \brief Проверка на равенство
    \param coef_1 Первое число
    \param coef_2 Второе число
    \details Проверяет, являются ли два числа NAN или сравнивает разницу двух аргументов с EPSILON - очень близким к нулю числом.
    \return true - если числа равны либо оба NAN
    \return false - если числа не равны
 */
bool isequal(const double coef_1, const double coef_2)
{
    return ((isnan(coef_1) && isnan(coef_2)) || fabs(coef_1 - coef_2) < EPSILON);
}


/**
    \brief Очистка буфера
    \details Создаёт переменную flag со значнием true. Если в строке остались символы помимо '\\n',
    запускает цикл, считывающий по одному символу и удаляющий их из буфера, пока не дойдет до символа '\\n',
    затем удаляет его и заканчивает работу.
    Если цикл был запущен и в строке остались не только пробелы, значение flag меняется на false.
    \return flag = true, если буфер не очищался
    \return flag = false, если буфер очищался
 */
bool clean_buf()
{
    bool flag = true;

    char c;
    while((c = getchar()) != '\n')
    {
        if (!isspace(c))
        {
            flag = false;
        }
    }

    return flag;
}
/** @} */


/**
    \defgroup output Вывод
    \brief Собрание функций вывода
    @{
 */
/**
    \brief Вывод корней
    \param consol_roots Структура, содержащая количество корней и корни уравнения
    \details Выводит количество и значения корней, соответствующее значению num_roots.
    \return 0 - успешное завершение работы
 */
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
/** @} */


/**
    \defgroup testing Тестирование
    \brief Собрание функций, используемых для проведения тестов
    @{
 */
/**
    \brief Проверка и запуск тестов
    \details checking_answer() проверяет, согласен ли пользователь запустить тесты,
    check_all_tests() возвращает false если хотя бы один тест не пройден.
 */
bool testing()
{
    return checking_answer() == 'Y' && !check_all_tests();
}


/**
    \brief Проверка ответа на вопрос про запуск тестов
    \details Запускается цикл, который запрашивает ответ и выдаёт ошибку, если введено не Y или N,
    работает, пока считываемый символ не будет Y или N.
    getchar() считывает код буквы, toupper() переводит её в верхний регистр.
    \return answer - ASCII код буквы ответа
 */
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


/**
    \brief Проверка тестов
    \details Создает переменные num_tests - общее количество тестов и success_tests - количество успешно пройденных тестов.
    Если количество успешных тестов не овпадает с общм количеством тестов, то выводит ошибку и возвращает false.
    В противном случае печатает, что все тесты пройдены верно и возвращает true.
    \return false - хотя бы один тест не пройден
    \return true - все тесты успешно пройдены
 */
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


/**
    \brief Определение значений для ручных тестов
    \details Задаёт 2 массива и переменную, массив test_coefs - все частные случаи коэффицентов, масссив test_roots - решения для них,
    переменная tests_success - количество успешных тестов.
    Затем запускает цикл, который функцией run_hand_test() по одному запускает все ручные тесты и, при успешном выполнении,
    увеличивает количество успешных тестов на 1.
    \return test_success - количество успешно выполненных тестов
 */

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
        {.coef_a = 1, .coef_b = 3,  .coef_c = 2},
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
        {.num_roots = TWO_SOLUTIONS,       .x1 = -3,   .x2 = -1},
        {.num_roots = ONE_SOLUTION,        .x1 = -5,  .x2 = -5 }
    };

    for (int num_test = 0; num_test  < NUM_HAND_TESTS; ++num_test)
    {
        test_success += run_hand_test(test_coefs[num_test], test_roots[num_test]);
    }

    return test_success;
}


/**
    \brief Запуск одного ручного теста
    \param test_coefs Структура, содержащая коэффиценты, по которым нужно вычислять корни
    \param test_roots Структура, содержащая корни, с которыми нужно сравнивать получнные
    \details Создаёт структуру check_roots, в которую с помощью solve() записывает значения корней и их количество.
    Сверяет значения корней из check_roots с ожидаемыми значениями из test_roots. В случае несовпадения выводит ошибку и возвращает 0.
    В противоположжном случае возвращает 1.
    \return 0 - тест провален
    \return 1 - тест пройден
 */
int run_hand_test(const coefficients_s test_coefs, const roots_s test_roots)
{
    roots_s check_roots = {.num_roots = INITIALIZ_VAL, .x1 = NAN, .x2 = NAN};
    solve(test_coefs, &check_roots);
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


/**
    \brief Проверка ручных тестов
    \param check_roots Структура, содержащая значения, подсчитанные программой
    \param test_roots Структура, содержащая значения, введённые от руки
    \details Сначала сравнивает количество корней, потом значение меньшего корня,
    потом значение большего корня. Если все правильно, возвращает true. В противном случае возвращает false.
    \return true - программа подсчитала верно, значения совпали
    \return false - программа подсчитала не верно, значения не совпали
 */

bool check_hand_test(const roots_s check_roots, const roots_s test_roots)
{
    return (check_roots.num_roots == test_roots.num_roots && isequal(check_roots.x1, test_roots.x1) && isequal(check_roots.x2, test_roots.x2));
}


/**
    \brief Создание рандомных тестов
    \details Начинает цикл, который запускает рандомные тесты по одному.
    Если тест успешен, то к количеству успешных прибавляется 1.
    \return success - количество успешных тестов
 */
int random_tests()
{
    srand(time(NULL));
    coefficients_s rand_coefs = {.coef_a = NAN, .coef_b = NAN, .coef_c = NAN};
    roots_s rand_roots = {.num_roots = INITIALIZ_VAL, .x1 = NAN, .x2 = NAN};
    int success = 0;
    for (int k = 0; k < NUM_RANDOM_TESTS; k++)
    {
        success += run_random_test(rand_coefs, &rand_roots);
    }

    return success;
}


/**
    \brief Запуск рандомного теста
    \param rand_coefs Структура, содержащая рандомные коэффиценты
    \param rand_roots Указатель на структуру, содержащую корни, вычисленные из рандомных коэффицентов
    \details Проверяет успешность теста с помощью check_random_test().
    В случае неуспеха, выводит ошибку и возвращает 0. В случае успеха возвращает 1.
    \return 1 - тест пройден
    \return 0 - тест не пройден
 */
bool run_random_test(coefficients_s rand_coefs, roots_s* const rand_roots)
{
    rand_coefs = {.coef_a = rand_double(), .coef_b = rand_double(), .coef_c = rand_double()};
    solve(rand_coefs, rand_roots);
    if (check_random_test(rand_coefs, *rand_roots))
    {
        return 1;
    }
    else
    {
        printf(RED "              Test FAILED\n" NO_COLOR
                "      a = %lg, b = %lg, c = %lg\n"
                "Expected: %d roots, x1 = %lg, x2 = %lg\n"
                "Got:      %d roots, x1 = %lg, x2 = %lg\n",\
                        rand_coefs.coef_a, rand_coefs.coef_b, rand_coefs.coef_c,\
                        rand_roots -> num_roots, rand_roots -> x1, rand_roots -> x2,\
                    rand_roots -> num_roots, rand_roots -> x1, rand_roots -> x2);
        return 0;
    }

}

/**
    \brief Проверка рандомного теста
    \param rand_coefs Структура, содержащая рандомные коэффиценты
    \param rand_roots Структура, содержащая корни, вычисленные из рандомных коэффицентов
    \details Первая проверка: если 0 корней, бесконечное количество решений или уравнение нельзя решить,
    то x1 и x2 проверяются на NAN. В случае успеха возвращается true.
    Вторая проверка: если 1 корень, то x2 проверяется на NAN и проверяется верность значения x1 с помощью check_sq_root()
    Третья проверка: если 2 корня, то проверяется верность значений x1 и x2 с помощью check_sq_root()
    \return true - проверка успешна
    \return false - проверка провалена
 */

bool check_random_test(const coefficients_s rand_coefs, roots_s const rand_roots)
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

/**
    \brief Проверка рандомных корней
    \param rand_coefs Структура, содержащая рандомные коэффиценты
    \param x - проверяемое значение корня
    \details Корень подставляется в квадратное уравнение с коэффицентами и его значение сравнивается с 0.
    Если они совпадают, то возвращается true. В противном случае возвращается false.
    \return true - значение x подходит
    \return false - значение x не подходит
 */

bool check_sq_root(const coefficients_s rand_coefs, const double x)
{
    if (isequal(rand_coefs.coef_a * x * x + rand_coefs.coef_b * x + rand_coefs.coef_c, 0))
    {
        return true;
    }

    return false;
}

/**
    \brief Генерация рандомного числа типа double
    \details В функции random_tests() устанавливается начало функции rand() в зависимости от количества секунд,
    прошедших с начала дня, чтобы числа были действительно случайными.
    RAND_MAX/(RAND_MAX + 1.0) превращает число типа int в число типа double.
    \return x - рандомное double число
 */

double rand_double()
{
    double x = RAND_MAX * (rand() / (RAND_MAX + 1.0));

    return x;
}
/** @} */
