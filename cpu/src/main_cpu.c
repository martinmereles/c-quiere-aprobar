#include "../include/main_cpu.h"

t_log* logger;
registros_t *reg;
pcb_t * contexto;
char* instruccion_exec;


int main(int argc, char* argv[]) {
	logger = iniciar_logger("./cfg/cpu-log.log", "cpu");
    t_config* config = iniciar_config(logger, "./cfg/cpu.config");
	reg = malloc(sizeof(registros_t));

	contexto = malloc(sizeof(pcb_t));
	instruccion_exec = malloc(sizeof(char));
	

	/*
	//Inicio hilo server
	char* puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
	char *param = (char*)malloc(sizeof(puerto));
	param=puerto;
	pthread_t hiloServidor;
	pthread_create(&hiloServidor,
                        NULL,
                        iniciar_hilo_server,
                        param);
	*/

	char* puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
	int socket_servidor = iniciar_servidor(puerto);
     pthread_t hiloAtencion;
     int *socket_cliente_kernel = (int)malloc(sizeof(int));
     socket_cliente_kernel = esperar_cliente(socket_servidor);
     pthread_create(&hiloAtencion,
                    NULL,
                    (void*) atender_cliente,
                    socket_cliente_kernel);
     pthread_detach(hiloAtencion);
	

	
	//Inicia conexion con memoria
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "La IP de Memoria es : %s", ip_memoria);
    log_info(logger, "El PUERTO de Memoria es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje("Me conecto desde CPU\n",socket_cliente_memoria);
	
	//Para prueba inicio
	contexto->pid =1;
	contexto->PCB_PC = 2;
	contexto->quantum = 2000;
	
	registros_t* prueba = malloc(sizeof(registros_t));
	prueba->AX = 3;
	prueba->BX = 4;
	prueba->PC = 0;
	contexto->reg_generales = prueba;

	reg->AX = 3;
	reg->BX = 4;
	reg->PC = 0;
	//Para prueba fin - revisar estructura

	while(1){
		fetch(socket_cliente_memoria);
		decode(socket_cliente_memoria);
		execute(socket_cliente_kernel);
		check_interrupt(socket_cliente_memoria);
	}

	pthread_join(hiloAtencion, NULL);
	//Cierre de log y config
	liberar_conexion(socket_cliente_memoria);
    cerrar_log_config (logger,config); 

	
	return EXIT_SUCCESS;
}

