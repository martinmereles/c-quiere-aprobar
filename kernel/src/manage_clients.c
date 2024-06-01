#include "../include/manage_clients.h"


void iniciar_hilo_server_kernel(char* puerto){
    int socket_servidor = iniciar_servidor(puerto);
    while (1) {
        pthread_t hiloAtencion;
        int *socket_cliente = (int)malloc(sizeof(int));
        socket_cliente = esperar_cliente(socket_servidor);
        pthread_create(&hiloAtencion,
                        NULL,
                        (void*) atender_cliente_kernel,
                        socket_cliente);
        pthread_detach(hiloAtencion);
    }
}

void atender_cliente_kernel(int socket_cliente){
	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(socket_cliente);; 
		switch (cod_op) {
		case MENSAJE:
            int size;
            char* buffer = recibir_buffer(&size, socket_cliente);
            log_info(logger, " %s", buffer);
            char ** mensaje_split = string_split(buffer, " ");
            if(strcmp(mensaje_split[0], "IO_GEN_SLEEP") == 0){
                io_gen_sleep(mensaje_split[1], mensaje_split[2], socket_cliente);

            }
            if(strcmp(mensaje_split[0], "CONECTAR_INTERFAZ") == 0){
                conectar_interfaz(mensaje_split[1], socket_cliente);
            }
            free(buffer);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente );
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto.");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}

void conectar_interfaz (char* tipo_interfaz, int socket_interfaz){
    t_interfaz* interfaz = malloc(sizeof(t_interfaz));

    interfaz->socket = socket_interfaz;
    interfaz->tipo_interfaz = tipo_interfaz;
    interfaz->identificador = proxima_interfaz_disponible();

    list_add (INTERFACES, interfaz);

    
}

void proxima_interfaz_disponible () {

}

void io_gen_sleep(char * interfaz, char* unidad_tiempo, int socket_cliente){

   
}

string_substring_from(char *text, int start);