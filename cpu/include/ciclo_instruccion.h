#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include <commons/string.h>
#include <../include/main_cpu.h>
#include <../include/auxiliares.h>
#include <../include/instrucciones_cpu.h>

extern pcb_t* contexto;
extern registros_t* reg;
extern char * instruccion_exec;
extern t_list* INTERRUPCIONES;

void fetch(int socket_cliente_memoria);
void decode(int socket_cliente_memoria, char* algoritmo_tlb, int cantidad_entradas_tlb);
void execute(int socket_cliente_kernel);
void check_interrupt(int socket_cliente_memoria, int socket_kernel_dispatch);
unsigned int get_valor_registro(char * registro);
#endif