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
	contexto->reg_generales = malloc (sizeof(registros_t));

	//Inicia servidor Puerto escucha dispatch
	char* puerto_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
	int socket_servidor_dispatch = iniciar_servidor(puerto_dispatch);
	pthread_t hilo_atencion_dispatch;
	int *socket_kernel_dispatch = (int)malloc(sizeof(int));
	socket_kernel_dispatch = esperar_cliente(socket_servidor_dispatch);
	pthread_create(&hilo_atencion_dispatch,
					NULL,
					(void*) atender_cliente_dispatch,
					socket_kernel_dispatch);
	pthread_detach(hilo_atencion_dispatch);

	//Inicia servidor Puerto escucha interrupt
	char* puerto_interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
	int socket_servidor_interrupt = iniciar_servidor(puerto_interrupt);
	pthread_t hilo_atencion_interrupt;
	int *socket_kernel_interrupt = (int)malloc(sizeof(int));
	socket_kernel_interrupt = esperar_cliente(socket_servidor_interrupt);
	pthread_create(&hilo_atencion_interrupt,
					NULL,
					(void*) atender_cliente,
					socket_kernel_interrupt);
	pthread_detach(hilo_atencion_interrupt);
	
	//Inicia conexion con memoria
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "La IP de Memoria es : %s", ip_memoria);
    log_info(logger, "El PUERTO de Memoria es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje("Me conecto desde CPU\n",socket_cliente_memoria);
	
	//Para prueba inicio
	contexto->pid =100;
	contexto->PCB_PC = 2;
	contexto->quantum = 2000;
	

	reg->AX = (uint8_t)1;
	reg->BX = (uint8_t)2;
	reg->PC = 0;
    reg->CX = (uint8_t)3;
    reg->DX = (uint8_t)4;
    reg->EAX = 5;
    reg->EBX = 6;
    reg->ECX = 7;
    reg->EDX = 8;
    reg->SI = 9;
    reg->DI = 10;

	contexto->reg_generales = reg;

	//PRUEBA DE ENVIO DE CONTEXTO A KERNEL
	enviar_pcb_contexto (socket_kernel_dispatch, contexto);

	//Para prueba fin - revisar estructura

	while(1){
		fetch(socket_cliente_memoria);
		decode(socket_cliente_memoria);
		execute(socket_kernel_dispatch);
		check_interrupt(socket_cliente_memoria);
	}

	//pthread_join(hilo_atencion_dispatch, NULL);
	//pthread_join(hilo_atencion_interrupt, NULL);
	//Cierre de log y config
	liberar_conexion(socket_cliente_memoria);
	liberar_conexion(socket_kernel_dispatch);
	liberar_conexion(socket_kernel_interrupt);
    cerrar_log_config (logger,config); 

	
	return EXIT_SUCCESS;
}

