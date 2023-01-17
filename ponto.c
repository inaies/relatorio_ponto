#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ponto.h>

int main()
{
    char entrada, saida;
    time_t seconds;
    char input;
    registro_t reg;
    FILE *registro; 

    //colocar numa funcao ? 
    registro = fopen("relatorio.txt", "a+");
    if(registro == NULL)
    {
        perror("Erro ao abrir arquivo de relatorio");
        exit(1);
    }

    scanf("%c", input);

    entrada = e;
    saida = s;

    for(;;)
    {
        if(!(strcmp(input, entrada)))
        {
            reg.inicial = time
        }
        if(!(strcmp(input, saida)))
        {
            reg.final = time;
            reg.res_diario = reg.res_diarioa + reg.final - reg.inicial;
        }
    }
}