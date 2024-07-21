#ifndef RECURSOS_H_
#define RECURSOS_H_

#include <commons/collections/list.h>
#include <stdlib.h>
#include <commons/string.h>
#include <utils/estructuras/estructuras.h>
#include <utils/client-server.h>
#include <semaphore.h>

extern int GLOBAL_PID;
extern t_list* QUEUE_NEW;
extern t_list* QUEUE_READY;
extern t_list* QUEUE_RUNNING;
extern t_list* QUEUE_BLOCKED;
extern t_list* QUEUE_TERMINATED;
extern t_sem_estados sem_array_estados[6];
extern sem_t sem_grado_multiprog;
extern t_list* lista_recursos;
extern int socket_cpu_interrupt;
extern int socket_memoria;

typedef struct 
{
    char* nombre;
    int instancias;
    t_list* procesos_bloqueados;
    sem_t mutex_recurso;
}recurso_t;

void cargar_recursos(t_config* config);
bool existe_recurso(char* recurso);
void wait_instuccion(char* nombre_recurso, int pid);
void signal_instuccion(char* nombre_recurso, int pid);
bool es_recurso_buscado(char* identificador, void* elemento);

#endif