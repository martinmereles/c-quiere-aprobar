#ifndef PLANIFICADOR_LARGO_PLAZO_H_
#define PLANIFICADOR_LARGO_PLAZO_H_

#include <commons/collections/list.h>
#include <stdlib.h>
#include <commons/string.h>
#include <utils/estructuras/estructuras.h>
#include <utils/client-server.h>
#include <semaphore.h>
#include <../include/manage_clients.h>


extern int GLOBAL_PID;
extern t_list* QUEUE_NEW;
extern t_list* QUEUE_READY;
extern t_list* QUEUE_RUNNING;
extern t_list* QUEUE_BLOCKED;
extern t_list* QUEUE_TERMINATED;
extern t_sem_estados sem_array_estados[6];
extern sem_t sem_grado_multiprog;
extern sem_t detencion_planificador_largo;
extern char* algoritmo;

void planificador_largo_plazo();
void iniciar_proceso (char* ruta, char* quantum, int socket_cliente_memoria);
pcb_t* crear_pcb (int quantum);
void pasar_new_ready ();

#endif