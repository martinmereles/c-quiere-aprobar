#ifndef PLANIFICADOR_CORTO_PLAZO_H_
#define PLANIFICADOR_CORTO_PLAZO_H_

#include <commons/log.h>
#include <commons/config.h>

#include <utils/client-server.h>
#include <utils/estructuras/estructuras.h>

extern t_log* logger;
extern sem_t sem_multiprocesamiento;
extern t_list* QUEUE_READY;
extern t_list* QUEUE_READY_PLUS;
extern t_list* QUEUE_RUNNING;
extern t_list* QUEUE_BLOCKED;
extern t_sem_estados sem_array_estados[6];
extern sem_t detencion_planificador_corto;
extern int socket_cpu_interrupt;
extern char* algoritmo;

void planificador_corto_plazo(socket_info_t* params);
void ejecutar_fifo(int socket_cpu_dispatch);
void ejecutar_round_robin(int socket_cpu_dispatch, int socket_cpu_interrupt, int quantum_int);
void ejecutar_virtual_rr(int socket_cpu_dispatch, int socket_cpu_interrupt, int quantum_int);
void dispatcher(pcb_t* pcb_a_enviar, int socket_cpu_dispatch);
void aviso_quantum(char* mensaje);
#endif