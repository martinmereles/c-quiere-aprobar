#ifndef AUXILIARES_H_
#define AUXILIARES_H_

#include <utils/client-server.h>
#include <../include/main_cpu.h>
#include <semaphore.h>

extern char* instruccion_exec;
extern pcb_t* contexto;
extern sem_t sem_execute;
void atender_cliente_cpu(int socket_cliente);
void atender_cliente_dispatch(int socket_cliente);
bool es_intruccion (char* palabra);

#endif