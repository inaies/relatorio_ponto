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

horario_t calcula_totais(FILE *registro)
{
    horario_t atual, mensal, anterior;
    char *linha;
    int aux;
    char *pt;
    int mes_atual, mes;
    long int size;
    char *limpa = "                      ";

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
        // size = ftell(registro);
        // if (strstr(linha, "Total mensal: "))
        // {
        //     fseek(registro, size, SEEK_SET);
        //     fwrite(limpa, sizeof(limpa), 1, registro);
        //     // mensal = soma_tempo(mensal, anterior);
        //     // printf("%02d:%02d:%02d", mensal.hor, mensal.min, mensal.sec);
        // }
        if (strstr(linha, "total: "))
        {
            pt = strtok(linha, "total: ");
            atual.hor = atoi(pt);
            pt = strtok(NULL, ":");
            atual.min = atoi(pt);
            pt = strtok(NULL, ":");
            atual.sec = atoi(pt);
            mensal = calcula_horas(atual, mensal, 1);
        }
        // printf("%02d:%02d:%02d", mensal.hor, mensal.min, mensal.sec);
    }

    free(linha);
    return(mensal);
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

        if(input == encerra)
            break;
    }

    fprintf(registro, "total: %02d:%02d:%02d\n", reg.diario.hor, reg.diario.min, reg.diario.sec);

    reg.mensal = calcula_totais(registro);
    rewind(registro);
    fprintf(registro, "Total mensal: %02d:%02d:%02d\n", reg.mensal.hor, reg.mensal.min, reg.mensal.sec);

    fclose(registro);

}