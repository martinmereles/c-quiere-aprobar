#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

#include <commons/collections/list.h>
#include <stdlib.h>
#include <commons/string.h>
#include "../include/main_kernel.h"
#include <utils/estructuras/estructuras.h>
#include <utils/client-server.h>

extern int GLOBAL_PID;
extern t_list* QUEUE_NEW;
extern t_list* QUEUE_READY;
extern t_list* QUEUE_RUNNING;
extern t_list* QUEUE_BLOCKED;
extern t_list* QUEUE_TERMINATED;

void iniciar_proceso (char* ruta, char* quantum, int socket_cliente_memoria);
pcb_t* crear_pcb (int quantum);

#endif