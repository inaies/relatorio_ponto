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

    //converte os tempos iniciais e finais em segundos 
    total_inicial = calcula_secs_totais(inicio);
    total_final = calcula_secs_totais(final);

    //caso a funcao seja usada para calcular a diferenca ou a soma entre dois tempos
    if(soma_diff)
        tempo_total = total_inicial + total_final;
    else
        tempo_total = total_final - total_inicial;
    

    //converte novamente os segundos totais no formato hh:mm:ss
    resul.hor = floor(tempo_total/3600);
    tempo_total = tempo_total % 3600;
    resul.min = floor(tempo_total/60);
    tempo_total = tempo_total % 60;
    resul.sec = tempo_total;

    return (resul);
}

void calcula_totais(FILE *registro, horario_t *total_mensal)
{
    horario_t mensal, atual;
    char *linha, *pt;
    int mes_atual;

    rewind(registro);

    mensal.hor = 0;
    mensal.min = 0;
    mensal.sec = 0;

    linha = malloc(sizeof(char) * linesize);

    //le todo o arquivo de registro de horas para somar as horas de cada mes
    while(fgets(linha, linesize, registro) != NULL)
    {
        if(strstr(linha, "Data: "))
        {
            pt = strtok(linha, "/");
            pt = strtok(NULL, "/");
            mes_atual = atoi(pt);
        }

        //busca todos os totais diarios de cada mes para soma-los no vetor 
        if(strstr(linha, "total: "))
        {
            pt = strtok(linha, "total: ");
            atual.hor = atoi(pt);
            pt = strtok(NULL, ":");
            atual.min = atoi(pt);
            pt = strtok(NULL, ":");
            atual.sec = atoi(pt);
            total_mensal[mes_atual] = calcula_horas(atual, total_mensal[mes_atual], 1);
        }
    }

    free(linha);
}

int main()
{
    FILE *registro;
    registro_t reg;
    horario_t *total_mensal;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char entrada, saida, encerra, input;
    int primeiro_reg = 1;
    int entrada_registrada = 0;
    int dia = tm.tm_mday;

    printf("e -> entrada \n s -> saida \n x -> termina programa \n");

    entrada = 'e';
    saida = 's';
    encerra = 'x';

    // abre, ou cria, caso nao exista, o arquivo .txt de relatorio
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

    //inicializa valores 
    zera_valores(total_mensal, &reg);

    for(;;)
    {
        scanf("%c", &input);
        t = time(NULL);
        tm = *localtime(&t);

        if(input == entrada)
        {
            entrada_registrada = 1;
            //verifica mudanca de dia
            if(tm.tm_mday != dia)
            {
                fprintf(registro, "total: %02d:%02d:%02d\n", reg.diario.hor, reg.diario.min, reg.diario.sec);
                fprintf(registro, "\nData: %02d/%02d/%d \n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
                reg.diario.hor = 0;
                reg.diario.min = 0;
                reg.diario.sec = 0;
            }
            if(primeiro_reg == 1)
            {
                fprintf(registro, "\nData: %02d/%02d/%d \n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
                primeiro_reg = 0;
                dia = tm.tm_mday;
            }

            //guarda no vetor as horas do sistema 
            reg.inicio.hor = tm.tm_hour;
            reg.inicio.min = tm.tm_min;
            reg.inicio.sec = tm.tm_sec;
            fprintf(registro, "     entrada: %02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
        }

        if(input == saida)
        {
            //verifica se houve uma entrada antes da saida
            if(entrada_registrada == 0)
                printf("saida registrada sem entrada previa, digite uma entrada primeiro \n");
            else
            {
                reg.final.hor = tm.tm_hour;
                reg.final.min = tm.tm_min;
                reg.final.sec = tm.tm_sec;
                dia = tm.tm_mday;

                fprintf(registro, "     saida: %02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

                //caso a saida seja registrada no dia seguinte, eh considerado como saida 23:59:59
                if(reg.final.hor < reg.inicio.hor)
                {
                    reg.final.hor = 23;
                    reg.final.min = 59;
                    reg.final.sec = 59;
                }

                //calcula diferenca de tempo entre entrada e saida
                reg.total = calcula_horas(reg.inicio, reg.final, 0);
                fprintf(registro, "     tempo: %02d:%02d:%02d\n", reg.total.hor, reg.total.min, reg.total.sec);

                //calcula soma de tempo trabalhada no dia
                reg.diario = calcula_horas(reg.diario, reg.total, 1);
                entrada_registrada = 0;
            }
        }

        if(input == encerra)
            break;
    }

    fprintf(registro, "total: %02d:%02d:%02d\n", reg.diario.hor, reg.diario.min, reg.diario.sec);

    calcula_totais(registro, total_mensal);

    //imprime total de horas de cada mes caso valor nao esteja zerado
    for(int i = 0; i < 12; i++)
        if(verifica_horas(total_mensal[i]))
            fprintf(registro, "\nTotal mensal de %s: %02d:%02d:%02d\n", converte_mes(i), total_mensal[i].hor, total_mensal[i].min, total_mensal[i].sec);

    free(total_mensal);
    fclose(registro);
}