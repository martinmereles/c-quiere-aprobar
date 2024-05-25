#include "../include/main_cpu.h"


t_log* logger;
registros_t *reg;

int main(int argc, char* argv[]) {
	logger = iniciar_logger("./cfg/cpu-log.log", "cpu");
    t_config* config = iniciar_config(logger, "./cfg/cpu.config");
	reg = malloc(sizeof(registros_t));
	
	//Inicio hilo server
	char* puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
	char *param = (char*)malloc(sizeof(puerto));
	param=puerto;
	pthread_t hiloServidor;
	pthread_create(&hiloServidor,
                        NULL,
                        iniciar_hilo_server,
                        param);
	
	
	//Inicia conexion con memoria
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "La IP de Memoria es : %s", ip_memoria);
    log_info(logger, "El PUERTO de Memoria es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje("Me conecto desde CPU\n",socket_cliente_memoria);

	enviar_mensaje("PROXIMA_INSTRUCCION 1 2",socket_cliente_memoria);

	atender_cliente(socket_cliente_memoria);
	
	pthread_join(hiloServidor, NULL);
	//Cierre de log y config
	liberar_conexion(socket_cliente_memoria);
    cerrar_log_config (logger,config); 

	
	return EXIT_SUCCESS;
}

