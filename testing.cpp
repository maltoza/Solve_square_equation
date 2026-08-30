#include "testing.h"

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
    printf(GREEN "Yes(Y)" NO_COLOR "/" RED "NO(N)\n" NO_COLOR);
    int answer = 0;
    answer = toupper(getchar());
    while (answer != 'Y' && answer != 'N')
    {
        printf(RED "Unknown command. Try again!\n" NO_COLOR);
        printf(YELLOW "Do you want to run tests before solving?\n" NO_COLOR);
        printf(GREEN "Yes(Y)" NO_COLOR "/" RED "NO(N)\n" NO_COLOR);
        clean_consol_buf();
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
    Вторая проверка: если 1 или 2 корня, то проверяется верность значений x1 и x2 с помощью check_sq_root(),
    т.к. при совпадении корней результат записвается в оба корня.
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
    else if ((rand_roots.num_roots == TWO_SOLUTIONS || rand_roots.num_roots == ONE_SOLUTION)
    && check_sq_root(rand_coefs, rand_roots.x1) && check_sq_root(rand_coefs, rand_roots.x2))
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
