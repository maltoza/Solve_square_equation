#include "test.h"
#include "anothers.h"

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

