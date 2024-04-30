#ifndef UTILS_ADMIN_H_
#define UTILS_ADMIN_H_
#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <commons/log.h>


void cerrar_log_config (t_log *, t_config *);


typedef struct pcb
{


    int pid;
    t_list* instrucciones; //lista de chars*
    int program_counter;
    char* estimado_rafaga;
//  t_registros_cpu *registros_cpu;
    int estimado_prox_rafaga;
    t_list* archivos_abiertos;
    t_list* t_segmentos;

}



#endif