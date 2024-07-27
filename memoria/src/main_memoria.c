#include "../include/main_memoria.h"


t_log* logger;
t_list* memoria_instrucciones;
int retardo_respuesta;
void* memoria;
int tamanio_pagina;
t_bitarray * bitmap_marcos_libres;
sem_t *sem_bitmap_marcos_libres;

int main(int argc, char* argv[]) {
	memoria_instrucciones = list_create();
	logger = iniciar_logger("./cfg/memoria-log.log", "memoria");
	t_config* config = iniciar_config(logger, "./cfg/memoria.config");
	sem_init(&sem_bitmap_marcos_libres, 0, 1);

	//Inicio de hilo server
	char* puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
	char* retardo = config_get_string_value(config, "RETARDO_RESPUESTA");

	retardo_respuesta = atoi(retardo);

	pthread_t hiloServidor;
	pthread_create(&hiloServidor,
                        NULL,
                        iniciar_hilo_server_memoria,
                        puerto);
	

	

	//Inicializamos espacio contiguo de memoria
	int tamanio_memoria = config_get_int_value(config, "TAM_MEMORIA");
	memoria = malloc(tamanio_memoria);
	tamanio_pagina = config_get_int_value(config, "TAM_PAGINA");

	int cantidad_marcos = tamanio_memoria/tamanio_pagina;
	int cantidad_marcos_bytes = cantidad_marcos/8;

	void * marcos_libres = malloc(cantidad_marcos_bytes);
	sem_wait(&sem_bitmap_marcos_libres);
	bitmap_marcos_libres = bitarray_create_with_mode(marcos_libres, cantidad_marcos_bytes, LSB_FIRST);
	//Seteamos en 0 el array
	for(int i = 0;i < cantidad_marcos;i++){
		bitarray_clean_bit(bitmap_marcos_libres, i);
	}
	sem_post(&sem_bitmap_marcos_libres);
	



	pthread_join(hiloServidor, NULL);
	//Cierre de log y config
    cerrar_log_config (logger,config); 
	free(memoria);
	list_destroy(memoria_instrucciones);
	return EXIT_SUCCESS;
}

