#include "../include/main_memoria.h"


t_log* logger;
t_list* memoria_instrucciones;
int retardo_respuesta;
void* memoria;

int main(int argc, char* argv[]) {
	memoria_instrucciones = list_create();
	logger = iniciar_logger("./cfg/memoria-log.log", "memoria");
	t_config* config = iniciar_config(logger, "./cfg/memoria.config");

	//Inicio de hilo server
	char* puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
	char* retardo = config_get_string_value(config, "RETARDO_RESPUESTA");

	retardo_respuesta = atoi(retardo);

	pthread_t hiloServidor;
	pthread_create(&hiloServidor,
                        NULL,
                        iniciar_hilo_server_memoria,
                        puerto);
	

	pthread_join(hiloServidor, NULL);

	//Inicializamos espacio contiguo de memoria
	int tamanio_memoria = config_get_int_value(config, "TAM_MEMORIA");
	memoria = malloc(tamanio_memoria);

	//Cierre de log y config
    cerrar_log_config (logger,config); 
	free(memoria);
	list_destroy(memoria_instrucciones);
	return EXIT_SUCCESS;
}

