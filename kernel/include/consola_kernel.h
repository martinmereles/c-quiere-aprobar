#ifndef CONSOLA_KERNEL_H_
#define CONSOLA_KERNEL_H_

#include <commons/collections/list.h>
#include <stdlib.h>
#include <commons/string.h>
#include "../include/main_kernel.h"
#include <utils/estructuras/estructuras.h>
#include <utils/client-server.h>
#include <utils/admin.h>
#include "planificador_largo_plazo.h"

extern int GLOBAL_PID;
extern t_list* QUEUE_NEW;
extern t_list* QUEUE_READY;
extern t_list* QUEUE_RUNNING;
extern t_list* QUEUE_BLOCKED;
extern t_list* QUEUE_TERMINATED;

void lanzar_consola (char* quantum, int socket_cliente_memoria, int socket_cpu_dispatch, t_config* config);
void entender_comando(char* command, char* quantum, int socket_cliente_memoria, t_config* config);
void ejecutar_script(char* path, char* quantum, int socket_cliente_memoria, t_config* config);
void finalizar_proceso(int pid);
void iniciar_planificacion();
bool es_pid_buscado(int pid, void* elemento);
#endif