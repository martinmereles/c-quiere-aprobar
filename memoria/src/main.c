#include <main.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils/client-server.h>

t_log* logger;
int main(int argc, char* argv[]) {
	logger = iniciar_logger("tp0-memoria-log.log", "memoria");
	t_config* config = iniciar_config(logger, "memoria.config");
	char* puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
    int socket_servidor = iniciar_servidor(puerto);
    int socket_cliente = esperar_cliente(socket_servidor);



t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente );
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;
}

void iterator(char* value) 
{
	log_info(logger,"%s", value);
}