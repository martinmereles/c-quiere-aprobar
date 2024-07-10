#ifndef MMU_H_
#define MMU_H_

#include <commons/collections/list.h>
#include <time.h>


extern t_list *tlb;

struct
{
    int pid;
    int pagina;
    int marco;
    time_t timestamp;
}entrada_tlb_t;

void iniciar_tlb(t_config config);
entrada_tlb_t * obtener_entrada(int pid, int pagina);
void remplazar_por_fifo(int tamanio_tlb, entrada_tlb_t * entrada):
void remplazar_por_lru(int tamanio_tlb, entrada_tlb_t * entrada);

#endif