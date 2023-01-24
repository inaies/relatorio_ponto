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


enum meses{
    janeiro,
    fevereiro,
    marco,
    abril,
    maio,
    junho,
    julho,
    agosto, 
    setembro,
    outubro,
    novembro,
    dezembro
};

#endif