
/// Погрешность, получаемая при сравнении чисел типа double
#define EPSILON 0.00000001
/// Количество ручных тестов
#define NUM_HAND_TESTS 10
/// Количество случайных тестов
#define NUM_RANDOM_TESTS 1000
/// Максимальная длина строки
#define MAX_LEN 50

/// Белый цвет
#define NO_COLOR "\033[0m"
/// Красный цвет
#define RED "\033[1;31m"
/// Жёлтый цвет
#define YELLOW "\033[1;33m"
/// Зелёный цвет
#define GREEN "\033[1;32m"
/// Фиолетовый цвет
#define PURPLE "\033[35m"


/**
    \brief Множество возможных количеств решений уравнения
    \details Количество решений квадратного уравнения (от 0 до 2),
    бесконечное множество решений = 3,
    невозможное количество корней = -1,
    уравнение, которое невозможно решить = 4
*/
enum numb_solutions
{
    INITIALIZ_VAL = -1,
    ZERO_SOLUTIONS,
    ONE_SOLUTION,
    TWO_SOLUTIONS,
    INFINITY_SOLUTIONS,
    IMPOSSIBLE_EQUATION,
};

/**
    \brief Структура параметров, необходимых для решения квадратного уравнения

*/

struct values_s
{
    numb_solutions num_roots; ///< Количество корней уравнения(принимает значения от -1 до 4)
    double coef_a, coef_b, coef_c;
    double x1, x2; ///< Больший корень
};

//TODO: две структуры

int solve                      (values_s* const consol_values);
numb_solutions line_roots      (const double coef_b,            const double coef_c);
numb_solutions partic_solutions(const double coef_c);
int line_eq                    (values_s* const consol_values);
double discriminant            (const values_s consol_values);
numb_solutions sq_roots        (const double discr);
int square_eq                  (values_s* const consol_values, const double discr);

bool isequal   (const double coef_1, const double coef_2);
bool clean_buf ();
