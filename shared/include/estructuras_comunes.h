#ifndef ESTRUCTURAS_COMUNES_H
#define ESTRUCTURAS_COMUNES_H

#include "typeDef.h"
#include "config.h"
#include "log.h"
#include "conexion.h"
#include "server.h"

//estructuras de PCB
/*--------------------------*/
typedef struct {
    long AX;
    long BX;
    long CX;
    long DX;
    uint8_t EAX;
    uint8_t EBX;
    uint8_t ECX;
    uint8_t EDX;
    uint16_t RAX;
    uint16_t RBX;
    uint16_t RCX;
    uint16_t RDX;
}t_registros_cpu;

typedef struct {
    int pid;
    t_list* instrucciones; //lista de chars*
    int program_counter;
    char* estimado_rafaga;
    t_registros_cpu *registros_cpu;
    int estimado_prox_rafaga;
    time_t ultimo_ready;
    t_list* archivos_abiertos;
    t_list* t_segmentos;
}t_pcb; //estructura de PCB, va a tener 2 estructuras dentro

/*--------------------------*/

// OPCODE DE INSTRUCCIONES
/*--------------------------*/

typedef enum op_code {
        SET,
        WAIT,
        I_O,
        SIGNAl,
        MOV_IN,
        F_OPEN,
        YIELD,
        F_TRUNCATE,
        F_SEEK,
        CREATE_SEGMENT,
        F_WRITE,
        F_READ,
        DELETE_SEGMENT,
        F_CLOSE,
        EXIT
};

/*--------------------------*/

//OP_CODEs para comunicacion entre consola y kernel
typedef enum op_code_consola{
    INSTR,
    PCB,
    INTERRUPCION
};

#endif