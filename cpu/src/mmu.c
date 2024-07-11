#include "../include/mmu.h"


void iniciar_tlb(){
    tlb = list_create();
}


entrada_tlb_t * obtener_entrada(int pid, int pagina){
    entrada_tlb_t * aux = malloc(sizeof(entrada_tlb_t));
    aux = NULL;
    entrada_tlb_t * entrada_buscada = malloc(sizeof(entrada_tlb_t));
    entrada_buscada = NULL;
    for(int i = 0;i < list_size(tlb); i++){
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
        int i = 1;
        for(i; i < list_size(tlb);i++){
            aux2 = list_get(tlb, i);
            if(aux2->timestamp < aux->timestamp){
                aux = aux2;
                posicion = i;
            }
        }
        list_replace(tlb, i, entrada);
    }
}

int traducir_a_direccion_fisica(int pid, int dir_logica, int socket_cliente, char * algoritmo_tlb, int tamanio_tlb){
    
    int numero_marco;
    int numero_pagina = floor(dir_logica / tamanio_pagina);
    int desplazamiento = dir_logica - numero_pagina * tamanio_pagina;

    entrada_tlb_t * entrada = malloc(sizeof(entrada_tlb_t));
    entrada = NULL;
    entrada = obtener_entrada(pid, numero_pagina);

    if(entrada != NULL){
        return (entrada->marco * tamanio_pagina) + desplazamiento;
    }else{
        char* mensaje = string_new();
        string_append(&mensaje, "OBTENER_MARCO ");
        string_append(&mensaje, string_itoa(pid));
        string_append(&mensaje, " ");
        string_append(&mensaje, string_itoa(numero_pagina));
        enviar_mensaje(mensaje, socket_cliente);
        int cod_op = recibir_operacion(socket_cliente);
        int size;
        char* buffer = recibir_buffer(&size, socket_cliente);
        char ** mensaje_split = string_split(buffer, " ");
        if(strcmp(mensaje_split[0], "OBTENER_MARCO") == 0){
            if(strcmp(mensaje_split[1], "SEGMENTATION_FAULT") == 0){
                return -1;
            }else{
                numero_marco = atoi(mensaje_split[1]);
            }
        }

        entrada->pid = pid;
        entrada->pagina = numero_pagina;
        entrada->marco = numero_marco;
        time(&entrada->timestamp);

        if(strcmp(algoritmo_tlb, "FIFO") == 0){
            remplazar_por_fifo(tamanio_tlb, entrada);
        }else{
            remplazar_por_lru(tamanio_tlb, entrada);
        }
        return (numero_marco * tamanio_pagina) + desplazamiento;
    }
}