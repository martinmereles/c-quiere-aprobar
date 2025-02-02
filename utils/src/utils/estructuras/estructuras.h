#ifndef UTILS_ESTRUCTURAS_H_
#define UTILS_ESTRUCTURAS_H_

#include <stdint.h>
#include <semaphore.h>
#include <commons/collections/list.h>
#include <pthread.h>

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
    uint32_t PC;
    uint32_t SI;
    uint32_t DI;
}registros_t;

typedef enum{
    AX,  //1
    BX,  //2
    CX,  //3
    DX,  //4
    EAX, //5
    EBX, //6
    ECX, //7
    EDX, //8
    PC,  //9
    SI,  //10
    DI   //11
}enum_registro;
typedef struct 
{
    int pid;
    uint32_t PCB_PC;
    int quantum;
    registros_t* reg_generales;
}pcb_t;
typedef struct 
{
    pcb_t* pcb;
    char* motivo;
}pcb_motivo_t;
typedef enum
{
    CPU,
    MEMORIA,
    ENTRADASALIDA,
    KERNEL
}e_tipo_modulo;

typedef struct
{
    int socket_fd;

} pthread_arg;

typedef struct
{
    int socket;
    char* tipo_interfaz;
    char* identificador;
    pthread_t id;
    t_list * queue_instrucciones;
    sem_t sem_uso;
    sem_t contador;
}t_interfaz;

typedef struct 
{
    sem_t mutex;
    sem_t contador;
}t_sem_estados;

typedef struct 
{
    int socket_dispatch;
    int socket_interrupt;
}socket_info_t;


#endif

