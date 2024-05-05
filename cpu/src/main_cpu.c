#include "../include/main_cpu.h"


t_log* logger;
registros_t *reg;

int main(int argc, char* argv[]) {
	logger = iniciar_logger("./cfg/cpu-log.log", "cpu");
    t_config* config = iniciar_config(logger, "./cfg/cpu.config");
	reg = malloc(sizeof(registros_t));

//ARRANCA BLOQUE DE PRUEBAS
	
	reg->PC = 12;//para pruebas
	printf("El valor de registro PC es: %d\n", reg->PC);
	set("PC","0");
	jnz("PC","11");
	printf("El valor de registro PC es: %d\n", reg->PC);


/*
	char* valor = config_get_string_value(config, "CLAVE");
    
	//Se conecta como cliente ante memoria
	char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
	char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	log_info(logger, "La IP de MEMORIA es : %s", ip_memoria);
    log_info(logger, "El PUERTO de MEMORIA es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje(valor,socket_cliente_memoria);
    paquete(socket_cliente_memoria);
    liberar_conexion(socket_cliente_memoria);


	//Inicia escucha servidor
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

	//Cierre de log y config
    cerrar_log_config (logger,config); 

	*/
	return EXIT_SUCCESS;
}

void iterator(char* value) 
{
	log_info(logger,"%s", value);
}

