#ifndef PLANIFICADOR_LARGO_PLAZO_H_
#define PLANIFICADOR_LARGO_PLAZO_H_

#include <commons/collections/list.h>
#include <stdlib.h>
#include <commons/string.h>
#include <utils/estructuras/estructuras.h>
//#include <../include/main_kernel.h>
#include <utils/client-server.h>
#include <semaphore.h>

typedef struct
{
    int socket;
    char* tipo_interfaz;
    char* identificador;
    sem_t sem_uso;
}t_interfaz;

typedef struct 
{
    sem_t mutex;
    sem_t contador;
}t_sem_estados;

extern int GLOBAL_PID;
extern t_list* QUEUE_NEW;
extern t_list* QUEUE_READY;
extern t_list* QUEUE_RUNNING;
extern t_list* QUEUE_BLOCKED;
extern t_list* QUEUE_TERMINATED;
extern t_sem_estados sem_array_estados[5];
extern sem_t sem_grado_multiprog;

void planificador_largo_plazo();
void iniciar_proceso (char* ruta, char* quantum, int socket_cliente_memoria);
pcb_t* crear_pcb (int quantum);
void pasar_new_ready ();

#endif