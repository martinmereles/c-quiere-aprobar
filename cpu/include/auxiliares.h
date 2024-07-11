#ifndef AUXILIARES_H_
#define AUXILIARES_H_

#include <utils/client-server.h>
#include <../include/main_cpu.h>
#include <semaphore.h>
#include <commons/temporal.h>

extern char* instruccion_exec;
extern pcb_t* contexto;
extern sem_t sem_execute;
extern int socket_kernel_dispatch;
extern t_list* INTERRUPCIONES;
extern t_temporal* temporizador;
extern int tamanio_pagina;

void atender_cliente_cpu(int socket_cliente);
void atender_cliente_dispatch(int socket_cliente);
bool es_intruccion (char* palabra);
void atender_cliente_interrupt(int socket_servidor_interrupt);
void guardar_tamanio_pagina(char* tamanio);

#endif