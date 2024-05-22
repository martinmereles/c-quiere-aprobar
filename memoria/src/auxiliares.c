#include "../include/auxiliares.h"

void iniciar_hilo_server_memoria(char* puerto){
    int socket_servidor = iniciar_servidor(puerto);
    while (1) {
        pthread_t hiloAtencion;
        int *socket_cliente = (int)malloc(sizeof(int));
        socket_cliente = esperar_cliente(socket_servidor);
        pthread_create(&hiloAtencion,
                        NULL,
                        (void*) atender_cliente_memoria,
                        socket_cliente);
        pthread_detach(hiloAtencion);
    }
}

void atender_cliente_memoria(int socket_cliente){
	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(socket_cliente);; 
		switch (cod_op) {
		case MENSAJE:
            int size;
            char* buffer = recibir_buffer(&size, socket_cliente);
            log_info(logger, "Me llego el mensaje %s", buffer);
            char ** mensaje_split = string_split(buffer, " ");
            if(strcmp(mensaje_split[0], "INICIAR_PROCESO") == 0){
                iniciar_proceso(mensaje_split[1], mensaje_split[2]);
            }
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

void iniciar_proceso(char* process_id, char* path){
    int process = atoi(process_id);
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    t_instruccion_memoria* instruccion = malloc(sizeof(t_instruccion_memoria));
    instruccion->lista_intrucciones = list_create();
    instruccion->process_id = process;
    char linea[256];
    while(fgets(linea, sizeof(linea), f) != NULL){
        list_add(instruccion->lista_intrucciones, linea);
        printf("Linea=>%s",linea);
    }
    fclose(f);
    list_add(lista_instrucciones, instruccion);
}
