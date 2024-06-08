#ifndef AUXILIARES_H_
#define AUXILIARES_H_

#include <utils/client-server.h>
#include <../include/main_cpu.h>

extern char* instruccion_exec;
extern pcb_t* contexto;

void atender_cliente_cpu(int socket_cliente);
bool es_intruccion (char* palabra);
void devolver_contexto_a_kernel (int socket_kernel);
void enviar_pcb_contexto(int socket_kernel);

#endif