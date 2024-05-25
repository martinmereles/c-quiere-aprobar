#include "../include/auxiliares.h"

void atender_cliente_cpu(int socket_cliente){
	t_list* lista;
    while(1){
        int cod_op = recibir_operacion(socket_cliente);; 
        switch (cod_op) {
        case MENSAJE:
            int size;
            char* buffer = recibir_buffer(&size, socket_cliente);
            log_info(logger, "Me llego el mensaje %s", buffer);
            free(instruccion_exec);
            instruccion_exec = malloc(sizeof(buffer));
            instruccion_exec = buffer;
            free(buffer);
            break;
        case PAQUETE:
            lista = recibir_paquete(socket_cliente );
            log_info(logger, "Me llegaron los siguientes valores:\n");
            list_iterate(lista, (void*) iterator);
            break;
        case -1:
            log_error(logger, "el cliente se desconecto.");
            return EXIT_FAILURE;
        default:
            log_warning(logger,"Operacion desconocida. No quieras meter la pata");
            break;
        }
    }
}