#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

int verifica_horas(horario_t total)
{
    if((total.hor != 0)||(total.min != 0)||(total.sec != 0))
        return 1;
    return 0;
}