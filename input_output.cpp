#include "input_output.h"


/**
    \defgroup enter Ввод
    \brief Собрание функций, отвечающих за ввод коэффицентов
    @{
 */
/**
    \brief Функция запроса ввода коэффицентов
    \param coefs Указатель на структуру коэффицентов consol_coef, созданную в main()
    \details Запрос на ввод коэффицентов квадратного уравнения. Вызывает функцию imput_answer() и
    в зависимости от выбранного  метода ввода коэффицентов либо вызывает функцию imput_terminal_coef() для считывания
    коэффицентов из командной строки, либо открывает файл с коэффицентами, проверяет на верность их написания
    (в случае ошибки останавливает программу)
    и с помощью функции input_file_coef() записывает их. Затем закрывает файл
    \return 0 - успешное завершение работы функции

 */
int input(coefficients_s* const coefs)
{
    int answer = input_answer();
    if (answer == 'C')
    {
        input_terminal_coef(&(coefs -> coef_a), "Enter the coefficient before x^2: ");
        input_terminal_coef(&(coefs -> coef_b), "Enter the coefficient before x: ");
        input_terminal_coef(&(coefs -> coef_c), "Enter the constant term: ");
    }
    else
    {
        FILE* f_enter = fopen(ENTER_FILE, "r");
        if (f_enter == NULL)
        {
            printf(RED "Error" NO_COLOR " in %s file, %s function, %d line\n", __FILE__, __FUNCTION__, __LINE__);
            return 1;
        }
        if (!input_file_coef(&(coefs -> coef_a), f_enter) || !input_file_coef(&(coefs -> coef_b), f_enter) ||
        !input_file_coef(&(coefs -> coef_c), f_enter))
        {
            return 1;
        }

        fclose(f_enter);
    }

    return 0;
}


/**
    \brief Выбор откуда вводить коэффиценты
    \details Спрашивает, откуда считывать коэффиценты: из файла или из командной строки.
    \return answer - ASCII код буквы ответа (F или C)
 */
int input_answer()
{
    clean_consol_buf();
    printf(YELLOW "How do you want to enter coefficients?\n" NO_COLOR);
    printf(BLUE "from file(F)" NO_COLOR "/" PURPLE "from command line(C)\n" NO_COLOR);
    int answer = 0;
    answer = toupper(getchar());
    while (answer != 'F' && answer != 'C')
    {
        printf(RED "Unknown command. Try again!\n" NO_COLOR);
        printf(YELLOW "How do you want to enter coefficients?\n" NO_COLOR);
        printf(BLUE "from file(F)" NO_COLOR "/" PURPLE "from command line(C)\n" NO_COLOR);
        clean_consol_buf();
        answer = toupper(getchar());
    }

    return answer;
}



/**
    \brief Функция считывания коэффицента квадратного уравнения
    \param coef Адрес коэффицента квадратного уравнения
    \param prompt Массив символов (строка), уточняющий, какой коэффицент нужно вводить
    \details Пишет в командную строку что нужно вводить, создаёт переменную flag,
    сигнализирующую о количестве правильно считанных слов.
    Запускает цикл, заканчивающийся, когда верно считано одно число и функция clean_consol_buf() (очистка буфера) вернула true.
    В цикле, если количество верно считанных чисел равно 0 (ни одного числа не считалось),
    очистка буфера запускается ещё раз, т.к. она не сработала в условии цикла из-за того,
    что первый аргумент логического "или" (||) вернул true.
    В цикле программа выводит ошибку ввода и предлагает ввести ещё раз нужный коэффицент.
    \return 0 - успешное завершение программы
 */
int input_terminal_coef(double* const coef, const char prompt[])
{
    assert(coef != NULL);

    printf("%s", prompt);
    int flag = 1;
    while ((flag = scanf("%lg", coef)) != 1 || clean_consol_buf() == false)
    {
        if (flag == 0)
        {
            clean_consol_buf();
        }
        printf(RED "Error" NO_COLOR " in %s file, %s function, %d line\n", __FILE__, __FUNCTION__, __LINE__);
        printf("%s", prompt);
    }

    return 0;
}


/**
    \brief Ввод коэффицента из файла
    \param coef Указатель на коэффицент, куда записывается значение из файла
    \param f_enter Указатель на файл, в котором находятся коэффиценты
    \details В случае, когда верно считалось не одно число или буфер очищался выводит ошибку.
    \return true - коэффицент успешно записан
    \return false - произошла ошибка, коэффицент не записан
 */

bool input_file_coef(double* const coef, FILE * f_enter)
{
    assert(coef != NULL);
    assert(f_enter != NULL);

    int flag = 1;
    if ((flag = fscanf(f_enter, "%lg", coef)) != 1 || clean_file_buf(f_enter) == false)
    {
        if (flag == 0)
        {
            clean_file_buf(f_enter);
        }
        printf(RED "Error" NO_COLOR " in %s file, %s function, %d line\n", __FILE__, __FUNCTION__, __LINE__);
        printf(RED "Enter only double numbers, one in line.\n" NO_COLOR);
        return false;
    }

    return true;
}

/** @} */



/**
    \defgroup output Вывод
    \brief Собрание функций вывода
    @{
 */
/**
    \brief Вывод корней
    \param roots Структура, содержащая количество корней и корни уравнения
    \details Выводит количество и значения корней, соответствующее значению num_roots.
    \return 0 - успешное завершение работы
 */
int output(const roots_s roots)
{
    assert(roots.num_roots != INITIALIZ_VAL);

    switch (roots.num_roots)
    {
        case ZERO_SOLUTIONS:  //no solutions
            printf("0 solutions\n");
            break;
        case ONE_SOLUTION:  //line equation - 1 root
            printf("Line equation: 1 solution\n");
            printf("x = %lg", roots.x1);
            break;
        case TWO_SOLUTIONS: //square equation - 2 roots
            printf("2 solutions:\n");
            printf("x1 = %lg\n", roots.x1);
            printf("x2 = %lg\n", roots.x2);
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
