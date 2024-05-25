#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include <commons/string.h>
#include <../include/main_cpu.h>
#include <../include/auxiliares.h>
#include <../include/instrucciones_cpu.h>

extern pcb_t* contexto;
extern registros_t* reg;
extern char * instruccion_exec;

void fetch(int socket_cliente_memoria);
void decode(int socket_cliente_memoria);
void execute();
void check_interrupt(int socket_cliente_memoria);

#endif