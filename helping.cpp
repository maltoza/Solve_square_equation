#include "helping.h"


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
bool clean_consol_buf()
{
    bool flag = true;

    int c;
    while((c = getchar()) != '\n')
    {
        if (!isspace(c))
        {
            flag = false;
        }
    }

    return flag;
}


/**
    \brief Очистка файлового буфера
    \param f_enter Указатель на файл, в котором записаны коэффиценты
    \details Цикл проходит по строке в файле, пока не встретит символ \\n, затем завершает работу.
    Если цикл запускался, то у переменной flag будет значение false, в противном случае true.
    \return flag = false - если буфер очищался
    \return flag = true - если буфер очищался
 */
bool clean_file_buf(FILE * f_enter)
{
    bool flag = true;

    int c;
    while((c = fgetc(f_enter)) != '\n')
    {
        if (!isspace(c))
        {
            flag = false;
        }
    }

    return flag;
}

/** @} */

