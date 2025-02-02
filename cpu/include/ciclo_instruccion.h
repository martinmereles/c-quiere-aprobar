#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include <commons/string.h>
#include <../include/main_cpu.h>
#include <../include/auxiliares.h>
#include <../include/instrucciones_cpu.h>
#include <../include/mmu.h>

extern pcb_t* contexto;
extern registros_t* reg;
extern char * instruccion_exec;
extern t_list* INTERRUPCIONES;
extern t_list* lista_marcos;
extern bool desalojo_out_of_memory;
extern t_list *lista_marcos_destino;

void fetch(int socket_cliente_memoria);
void decode(int socket_cliente_memoria, char* algoritmo_tlb, int cantidad_entradas_tlb);
void execute(int socket_cliente_kernel, int socket_cliente_memoria);
void check_interrupt(int socket_cliente_memoria, int socket_kernel_dispatch);
unsigned int get_valor_registro(char * registro);
int get_tamanio_registro(char * registro);
void set_valor_registro(char * registro, int dato);
#endif