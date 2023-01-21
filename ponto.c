#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "lib.h"
#define linesize 1024

int calcula_secs_totais(horario_t *tempo)
{
    return (tempo->hor * 3600 + tempo->min * 60 + tempo->sec);
}

void calcula_horas(registro_t *reg)
{
    int total_inicial, total_final, diff;

    total_inicial = calcula_secs_totais(&reg->inicio);
    total_final = calcula_secs_totais(&reg->final);

    diff = total_final - total_inicial;
    
    reg->total.hor = floor(diff/3600);
    diff = diff % 3600;
    reg->total.min = floor(diff/60);
    diff = diff % 60;
    reg->total.sec = diff;
}

void soma_tempo(horario_t *atual, horario_t *soma)
{
    int atual_total, soma_total;
    atual_total = calcula_secs_totais(atual);
    soma_total = calcula_secs_totais(soma);

    soma_total += atual_total;

    soma->hor = floor(soma_total/3600);
    soma_total = soma_total % 3600;
    soma->min = floor(soma_total/60);
    soma_total = soma_total % 60;
    soma->sec = soma_total;
}

int calcula_totais(FILE *registro)
{
    horario_t atual, soma;
    char *linha;
    int aux;

    linha = malloc(sizeof(char) * linesize);

    while(fgets(linha, linesize, registro) != NULL)
    {
        if(!(strstr(linha, "/"))&&(strcmp(linha, "\n")))
        {
            for(int i=0; i<6; i++)
                fscanf(registro, "%d", &aux);
            fscanf(registro, "%d %d %d", &atual.hor, &atual.min, &atual.sec);
        }

        soma_tempo(&atual, &soma);
    }
}


int main()
{
    char entrada, saida, encerra;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char input;
    registro_t reg;
    FILE *registro;
    int primeiro_reg = 1;

    //ler somas diarias pelo txt usando strtok
    //ler total diaria usando strstr (buscando pela palavra "total")

    printf("%02d:%02d:%02d \n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("e -> entrada \n s -> saida \n x -> termina programa \n");

    entrada = 'e';
    saida = 's';
    encerra = 'x';

    // colocar numa funcao ? 
    registro = fopen("relatorio.txt", "a+");
    if(registro == NULL)
    {
        perror("Erro ao abrir arquivo de relatorio");
        exit(1);
    }

    for(;;)
    {
        scanf("%c", &input);
        t = time(NULL);
        tm = *localtime(&t);

        if(input == entrada)
        {
            if((primeiro_reg == 1))
            {
                fprintf(registro, "%02d/%02d/%d \n", tm.tm_mday, tm.tm_mon+1, tm.tm_year + 1900);
                primeiro_reg = 0;
            }
            reg.inicio.hor = tm.tm_hour;
            reg.inicio.min = tm.tm_min;
            reg.inicio.sec = tm.tm_sec;
            fprintf(registro, "     entrada: %02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
        }

        if(input == saida)
        {
            reg.final.hor = tm.tm_hour;
            reg.final.min = tm.tm_min;
            reg.final.sec = tm.tm_sec;
            fprintf(registro, "     saida: %02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
            calcula_horas(&reg);
            fprintf(registro, "     tempo: %02d:%02d:%02d\n", reg.total.hor, reg.total.min, reg.total.sec);
        }

        if(input == encerra)
            break;
    }
    
    rewind(registro);
}