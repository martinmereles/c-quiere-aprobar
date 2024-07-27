#ifndef MMU_H_
#define MMU_H_

#include <commons/collections/list.h>
#include <time.h>
#include <commons/log.h>

extern t_log* logger;
extern t_list *tlb;
extern int tamanio_pagina;
extern t_list* lista_marcos;
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
int obtener_marco(int pid, int numero_pagina, int socket_cliente, char * algoritmo_tlb, int tamanio_tlb);
int calcular_cant_pag(int desplazamiento, int tam_registro);
void marcos_a_leer(int pid, unsigned int dir_logica, int tam_registro, int socket_cliente, char * algoritmo_tlb, int tamanio_tlb);
int calcular_desplazamiento(int dir_logica, int numero_pagina);
int calcular_num_pagina(int dir_logica);

#endif