#include "../include/main_memoria.h"


t_log* logger;
t_list* memoria_instrucciones;
int retardo_respuesta_sec;
int main(int argc, char* argv[]) {
	memoria_instrucciones = list_create();
	logger = iniciar_logger("./cfg/memoria-log.log", "memoria");
	t_config* config = iniciar_config(logger, "./cfg/memoria.config");

	//Inicio de hilo server
	char* puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
	char* retardo = config_get_string_value(config, "RETARDO_RESPUESTA");

	retardo_respuesta_sec = atoi(retardo)/1000;

	pthread_t hiloServidor;
	pthread_create(&hiloServidor,
                        NULL,
                        iniciar_hilo_server_memoria,
                        puerto);
	

	pthread_join(hiloServidor, NULL);
	//Cierre de log y config
    cerrar_log_config (logger,config); 
	list_destroy(memoria_instrucciones);
	return EXIT_SUCCESS;
}

