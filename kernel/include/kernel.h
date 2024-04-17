#ifndef KERNEL_H
#define KERNEL_H
#include "estructuras_comunes.h"
#include <stdio.h>
#include <commons/log.h>
#include <stdbool.h>
#include "shared_utils.h"
#include <commons/config.h>

//#include "typeDef.h"
//#include "kernel_utils.h"


void atender_cliente(void *arg);
void atencion_consola(void* arg);
typedef struct {
    char* ip_memoria;
    char* puerto_memoria;
    char* puerto_cpu_dispatch;
    char* ip_cpu;
    char* puerto_escucha;
    char* puerto_cpu_interrupt;
    char* algoritmo_planificacion;
    char* grado_multiprogramacion;
    char* quantum;
    char* recursos;
    char* instancias_recursos;
}t_kernel_config;

t_kernel_config* levantar_config_kernel ();

//funciones para recibir datos de consola
t_list *recibir_instrucciones(int socket_cliente, t_log* logger);

//Funcion de atencion a CPU

#endif