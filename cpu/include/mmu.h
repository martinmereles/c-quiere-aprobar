#ifndef MMU_H_
#define MMU_H_

#include <commons/collections/list.h>
#include <time.h>


extern t_list *tlb;
extern int tamanio_pagina;
typedef struct
{
    int pid;
    int pagina;
    int marco;
    time_t timestamp;
}entrada_tlb_t;

void iniciar_tlb();
entrada_tlb_t * obtener_entrada(int pid, int pagina);
void remplazar_por_fifo(int tamanio_tlb, entrada_tlb_t * entrada);
void remplazar_por_lru(int tamanio_tlb, entrada_tlb_t * entrada);
int traducir_a_direccion_fisica(int pid, unsigned int dir_logica, int socket_cliente, char * algoritmo_tlb, int tamanio_tlb);

#endif