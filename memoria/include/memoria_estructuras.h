#ifndef MEMORIA_ESTRUCTURAS_H_
#define MEMORIA_ESTRUCTURAS_H_

#include <commons/collections/list.h>

typedef struct{
    int process_id;
    t_list* lista_instrucciones;
}t_instruccion_memoria;

typedef struct{
    char* puerto;
    char* retardo_respuesta;
}t_parametros_memoria;

#endif