#ifndef HELPING_H
#define HELPING_H

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "general.h"


bool isequal          (const double coef_1, const double coef_2);
bool clean_consol_buf ();
bool clean_file_buf   (FILE * f_enter);

#endif //HELPING_H
