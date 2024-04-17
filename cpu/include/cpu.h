
#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <commons/log.h>
#include <stdbool.h>
#include <hello.h>
#include "shared_utils.h"
#include "typeDef.h"
#include "conexion.h"
#include "server.h"
#include "config.h"
#include "log.h"


void paquete(int conexion);
void atender_kernel(void *arg);
typedef struct 
{
    char* puerto_escucha;
    char* tam_memoria;
    char* tam_pagina;
    char* path_instrucciones;
    char* retardo_respuesta
}t_config_cpu;

t_config_cpu* levantar_config_cpu (void);



#endif