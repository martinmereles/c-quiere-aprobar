#include "../include/main_kernel.h"

t_log* logger;
int GLOBAL_PID;
t_list* QUEUE_NEW;
t_list* QUEUE_READY;
t_list* QUEUE_RUNNING;
t_list* QUEUE_BLOCKED;
t_list* QUEUE_TERMINATED;

int main(int argc, char* argv[]) {
    QUEUE_NEW = list_create();
    QUEUE_READY = list_create();
    QUEUE_RUNNING = list_create();
    QUEUE_BLOCKED = list_create();
    QUEUE_TERMINATED = list_create();
    
    logger = iniciar_logger("./cfg/kernel-log.log", "kernel");
    t_config* config = iniciar_config(logger, "./cfg/kernel.config");

    char* quantum = config_get_string_value(config, "QUANTUM");

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
    log_info(logger, "La IP de MEMORIA es : %s", ip_memoria);
    log_info(logger, "El PUERTO de MEMORIA es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje("Me conecto desde Kernel",socket_cliente_memoria);

    //Inicia conexion con cpu
    char* ip_cpu = config_get_string_value(config, "IP_CPU");
    char* puerto_cpu = config_get_string_value(config, "PUERTO_CPU");
    log_info(logger, "La IP de CPU es : %s", ip_cpu);
    log_info(logger, "El PUERTO de CPU es : %s", puerto_cpu);
    int socket_cliente_cpu = crear_conexion(ip_cpu,puerto_cpu);
    enviar_mensaje("Me conecto desde Kernel",socket_cliente_cpu);
    

    lanzar_consola(quantum, socket_cliente_memoria, config);
    
    pthread_join(hiloServidor, NULL);
    //Cierre de log y config
    cerrar_log_config (logger,config); 
    list_destroy(QUEUE_NEW);
    list_destroy(QUEUE_READY);
    list_destroy(QUEUE_RUNNING);
    list_destroy(QUEUE_BLOCKED);
    list_destroy(QUEUE_TERMINATED);
    
    return EXIT_SUCCESS;
}