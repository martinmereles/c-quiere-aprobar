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

typedef struct {
    uint32_t PC
    uint8_t AX;
    uint8_t BX;
    uint8_t CX;
    uint8_t DX;
    uint32_t EAX;
    uint32_t EBX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t SI;
    uint32_t DI;
}t_registros_cpu;

#endif