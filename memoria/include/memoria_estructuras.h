#ifndef MEMORIA_ESTRUCTURAS_H_
#define MEMORIA_ESTRUCTURAS_H_

#include <commons/collections/list.h>

typedef struct{
    int process_id;
    t_list* lista_intrucciones;
}t_instruccion_memoria;

#endif