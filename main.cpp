#include "general.h"
#include "input_output.h"
#include "solving.h"
#include "helping.h"
#include "testing.h"
//--------------------------------------------------------------------------------------------------------

int main()
{
    coefficients_s coefs = {.coef_a = NAN, .coef_b = NAN, .coef_c = NAN};
    roots_s roots = {.num_roots = INITIALIZ_VAL, .x1 = NAN, .x2 = NAN};

    if (testing())
    {
        printf(RED "Error" NO_COLOR " in %s file, %s function, %d line\n", __FILE__, __FUNCTION__, __LINE__);
        return 1;
    }

    if (input(&coefs) == 1)
    {
        printf(RED "Error" NO_COLOR " in %s file, %s function, %d line\n", __FILE__, __FUNCTION__, __LINE__);
        return 1;
    }

    solve(coefs, &roots);

    output(roots);

    return 0;
}

//--------------------------------------------------------------------------------------------------------

