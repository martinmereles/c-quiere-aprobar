#include "../include/mmu.h"


void iniciar_tlb(){
    tlb = list_create();
}


entrada_tlb_t * obtener_entrada(int pid, int pagina){
    entrada_tlb_t * aux = malloc(sizeof(entrada_tlb_t));
    aux = NULL;
    entrada_tlb_t * entrada_buscada = malloc(sizeof(entrada_tlb_t));
    entrada_buscada = NULL;
    for(int i = 0,i < list_size(tlb); i++){
        aux = list_get(tlb, i);
        if(aux->pid == pid && aux->pagina == pagina){
            time(&aux->timestamp);
            list_replace(tlb, i, aux);
            entrada_buscada = aux;
        }
    }

    return entrada_buscada;    
}

void remplazar_por_fifo(int tamanio_tlb, entrada_tlb_t * entrada){
    if(list_size(tlb) < tamanio_tlb){
        list_add(tlb, entrada);
    }else{
        list_remove(tlb, 0);
        list_add(tlb, entrada);
    }
}

void remplazar_por_lru(int tamanio_tlb, entrada_tlb_t * entrada){
    if(list_size(tlb) < tamanio_tlb){
        list_add(tlb, entrada);
    }else{
        entrada_tlb_t * aux = malloc(sizeof(entrada_tlb_t));
        entrada_tlb_t * aux2 = malloc(sizeof(entrada_tlb_t));
        aux = list_get(tlb, 0);
        int posicion = 0;
        for(int i = 1;i < list_size(tlb);i++){
            aux2 = list_get(tlb, i);
            if(aux2->timestamp < aux->timestamp){
                aux = aux2;
                posicion = i;
            }
        }
        list_replace(tlb, i, entrada);
    }
}
/*

número_página = floor(dirección_lógica / tamaño_página)
desplazamiento = dirección_lógica - número_página * tamaño_página
*/