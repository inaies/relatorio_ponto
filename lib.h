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
    int res_diario;
};
typedef struct registro registro_t;

#endif