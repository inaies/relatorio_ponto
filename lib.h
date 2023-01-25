#ifndef _lib_t_H
#define _lib_t_H

struct data
{
    int mes, ano, dia;
};
typedef struct data data_t;

struct horario
{
    int hor, min, sec;
};
typedef struct horario horario_t;

struct registro
{
    horario_t inicio, final, total;
    horario_t diario, mensal;
};
typedef struct registro registro_t;

int verifica_horas(horario_t total);

char *converte_mes(int num);

void zera_valores(horario_t *total_mensal, registro_t *reg);

#endif