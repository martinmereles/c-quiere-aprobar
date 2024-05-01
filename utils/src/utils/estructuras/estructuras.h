#ifndef UTILS_ESTRUCTURAS_H_
#define UTILS_ESTRUCTURAS_H_

#include <stdint.h>
typedef struct
{
    uint8_t AX;
    uint8_t BX;
    uint8_t CX;
    uint8_t DX;
    uint32_t EAX;
    uint32_t EBX;
    uint32_t ECX;
    uint32_t EDX;
}reg_general_t;

typedef struct 
{
    uint32_t PC;
    uint32_t SI;
    uint32_t DI;
    reg_general_t *reg_generales;
}registros_t;


typedef struct 
{
    int pid;
    uint32_t PCB_PC;
    int quantum;
    reg_general_t *reg_generales;
}pcb_t;


#endif

