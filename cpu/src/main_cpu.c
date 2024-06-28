#include "../include/main_cpu.h"

t_log* logger;
registros_t *reg;
pcb_t * contexto;
char* instruccion_exec;
sem_t sem_execute;
int socket_kernel_dispatch;
t_list* INTERRUPCIONES;
t_temporal* temporizador;

int main(int argc, char* argv[]) {
	INTERRUPCIONES = list_create();
	socket_kernel_dispatch = -1;
	logger = iniciar_logger("./cfg/cpu-log.log", "cpu");
    t_config* config = iniciar_config(logger, "./cfg/cpu.config");
	reg = malloc(sizeof(registros_t));
	contexto = malloc(sizeof(pcb_t));
	instruccion_exec = malloc(sizeof(char));
	contexto->reg_generales = malloc (sizeof(registros_t));
	sem_init(&sem_execute,0,0);

	//Inicia servidor Puerto escucha dispatch
	char* puerto_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
	int socket_servidor_dispatch = iniciar_servidor(puerto_dispatch);
	pthread_t hilo_atencion_dispatch;
	pthread_create(&hilo_atencion_dispatch,
					NULL,
					(void*) atender_cliente_dispatch,
					socket_servidor_dispatch);
	pthread_detach(hilo_atencion_dispatch);

	//Inicia servidor Puerto escucha interrupt
	char* puerto_interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
	int socket_servidor_interrupt = iniciar_servidor(puerto_interrupt);
	pthread_t hilo_atencion_interrupt;
	pthread_create(&hilo_atencion_interrupt,
					NULL,
					(void*) atender_cliente_interrupt,
					socket_servidor_interrupt);
	pthread_detach(hilo_atencion_interrupt);
	
	//Inicia conexion con memoria
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "La IP de Memoria es : %s", ip_memoria);
    log_info(logger, "El PUERTO de Memoria es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje("Me conecto desde CPU\n",socket_cliente_memoria);
	
	//Seteo inicial contexto
	contexto->pid =0;
	contexto->PCB_PC = 0;
	contexto->quantum = 0;
	reg->AX = (uint8_t)0;
	reg->BX = (uint8_t)0;
    reg->CX = (uint8_t)0;
    reg->DX = (uint8_t)0;
    reg->EAX = 0;
    reg->EBX = 0;
    reg->ECX = 0;
    reg->EDX = 0;
	reg->PC = 0;
    reg->SI = 0;
    reg->DI = 0;
	contexto->reg_generales = reg;


	sem_wait(&sem_execute);
	

	//Inicio de ciclo de instruccion
	while(1){
		fetch(socket_cliente_memoria);
		decode(socket_cliente_memoria);
		execute(socket_kernel_dispatch);
		check_interrupt(socket_cliente_memoria, socket_kernel_dispatch);
	}

	//pthread_join(hilo_atencion_dispatch, NULL);
	//pthread_join(hilo_atencion_interrupt, NULL);

	//Cierre de log y config
	liberar_conexion(socket_cliente_memoria);
	liberar_conexion(socket_kernel_dispatch);
    cerrar_log_config (logger,config); 
	
	return EXIT_SUCCESS;
}

