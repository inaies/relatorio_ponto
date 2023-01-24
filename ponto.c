#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "lib.h"
#define linesize 1024

int calcula_secs_totais(horario_t tempo)
{
    return (tempo.hor * 3600 + tempo.min * 60 + tempo.sec);
}

horario_t calcula_horas(horario_t inicio, horario_t final, int soma_diff)
{
    int total_inicial, total_final, tempo_total;
    horario_t resul;

    total_inicial = calcula_secs_totais(inicio);
    total_final = calcula_secs_totais(final);

    if(soma_diff)
        tempo_total = total_inicial + total_final;
    else
        tempo_total = total_final - total_inicial;
    
    resul.hor = floor(tempo_total/3600);
    tempo_total = tempo_total % 3600;
    resul.min = floor(tempo_total/60);
    tempo_total = tempo_total % 60;
    resul.sec = tempo_total;
    return (resul);
}

void calcula_totais(FILE *registro, horario_t *total_mensal)
{
    horario_t mensal, atual, anterior;
    char *linha;
    int aux;
    char *pt;
    int mes_atual, mes;
    long int size;

    rewind(registro);

    mensal.hor = 0;
    mensal.min = 0;
    mensal.sec = 0;

    linha = malloc(sizeof(char) * linesize);

    while(fgets(linha, linesize, registro) != NULL)
    {
        if(strstr(linha, "Data: "))
        {
            pt = strtok(linha, "/");
            pt = strtok(NULL, "/");
            mes_atual = atoi(pt);
            if(mes_atual != mes)
            {
                mensal.hor = 0;
                mensal.min = 0;
                mensal.sec = 0;
                mes = mes_atual;
            }
        }

        if (strstr(linha, "total: "))
        {
            pt = strtok(linha, "total: ");
            atual.hor = atoi(pt);
            pt = strtok(NULL, ":");
            atual.min = atoi(pt);
            pt = strtok(NULL, ":");
            atual.sec = atoi(pt);
            total_mensal[mes_atual] = calcula_horas(atual, mensal, 1);
        }
    }

    free(linha);
}

int main()
{
    char entrada, saida, encerra, resumo;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char input;
    registro_t reg;
    horario_t *total_mensal;
    enum meses mes;
    FILE *registro;
    int primeiro_reg = 1;

    printf("e -> entrada \n s -> saida \n x -> termina programa \n");

    entrada = 'e';
    saida = 's';
    resumo = 'r';
    encerra = 'x';

    // colocar numa funcao ? 
    registro = fopen("relatorio.txt", "a+");
    if(registro == NULL)
    {
        perror("Erro ao abrir arquivo de relatorio");
        exit(1);
    }

    //aloca vetor de struct horario_t para resumo de cada mes
    total_mensal = malloc(sizeof(horario_t)*12);
    if(total_mensal == NULL)
    {
        perror("erro ao alocar vetor de struct");
        exit(1);
    }

    reg.diario.hor = 0;
    reg.diario.min = 0;
    reg.diario.sec = 0;

    for(;;)
    {
        scanf("%c", &input);
        t = time(NULL);
        tm = *localtime(&t);

        if(input == entrada)
        {
            if((primeiro_reg == 1))
            {
                fprintf(registro, "\n");
                fprintf(registro, "Data: %02d/%02d/%d \n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
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
            reg.total = calcula_horas(reg.inicio, reg.final, 0);
            fprintf(registro, "     tempo: %02d:%02d:%02d\n", reg.total.hor, reg.total.min, reg.total.sec);

            reg.diario = calcula_horas(reg.diario, reg.total, 1);
        }

        if(input == resumo)
            break;

        if(input == encerra)
            break;
    }

    if(input != resumo)
        fprintf(registro, "total: %02d:%02d:%02d\n", reg.diario.hor, reg.diario.min, reg.diario.sec);

    calcula_totais(registro, total_mensal);
    for(int i = 0; i < 12; i++)
    {
        mes = i;
        if(verifica_horas(total_mensal[i]))
            fprintf(registro, "Total mensal de %s: %02d:%02d:%02d\n", mes, total_mensal[i].hor, total_mensal[i].min, total_mensal[i].sec);
    }

    free(total_mensal);
    fclose(registro);

}