#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

//verifica se o tempo esta zerado
int verifica_horas(horario_t total)
{
    if((total.hor != 0)||(total.min != 0)||(total.sec != 0))
        return 1;
    return 0;
}

//inicializa em zero totais mensais e diarios para eliminar erros de lixo de memoria 
void zera_valores(horario_t *total, registro_t *reg)
{
    for (int i = 0; i < 12; i++)
    {
        total[i].hor = 0;
        total[i].min = 0;
        total[i].sec = 0;
    }

    reg->diario.hor = 0;
    reg->diario.min = 0;
    reg->diario.sec = 0;

}

//retorna nome do mes conforme seu numero para melhorar legibilidade do relatorio
char* converte_mes(int num)
{
    switch(num)
    {
        case 1:
            return "Janeiro";
        case 2:
            return "Fevereiro";
        case 3:
            return "Marco";
        case 4:
            return "Abril";
        case 5:
            return "Maio";
        case 6:
            return "Junho";
        case 7:
            return "Julho";
        case 8:
            return "Agosto";
        case 9:
            return "Setembro";
        case 10:
            return "Outubro";
        case 11:
            return "Novembro";
        case 12:
            return "Dezembro";
    }
}