#include "../include/main_memoria.h"


t_log* logger;

int main(int argc, char* argv[]) {
	logger = iniciar_logger("./cfg/memoria-log.log", "memoria");
	t_config* config = iniciar_config(logger, "./cfg/memoria.config");

	//Inicio de hilo server // OJO QUE EN FUNCION COME MEMORIA
	char* puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
	char *param = (char*)malloc(sizeof(puerto));
	param=puerto;
	pthread_t hiloServidor;
	pthread_create(&hiloServidor,
                        NULL,
                        iniciar_hilo_server,
                        param);
	

	pthread_join(hiloServidor, NULL);
	//Cierre de log y config
    cerrar_log_config (logger,config); 

	return EXIT_SUCCESS;
}

