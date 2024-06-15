#include "../include/main_kernel.h"

t_log* logger;
int GLOBAL_PID;
t_list* QUEUE_NEW;
t_list* QUEUE_READY;
t_list* QUEUE_RUNNING;
t_list* QUEUE_BLOCKED;
t_list* QUEUE_TERMINATED;
t_list* INTERFACES;

int main(int argc, char* argv[]) {
    QUEUE_NEW = list_create();
    QUEUE_READY = list_create();
    QUEUE_RUNNING = list_create();
    QUEUE_BLOCKED = list_create();
    QUEUE_TERMINATED = list_create();
    INTERFACES = list_create();
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
                        iniciar_hilo_server_kernel,
                        param);
	pthread_detach(hiloServidor);
    
    //Inicia conexion con memoria
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "La IP de MEMORIA es : %s", ip_memoria);
    log_info(logger, "El PUERTO de MEMORIA es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje("Me conecto desde Kernel",socket_cliente_memoria);

    
    char* ip_cpu = config_get_string_value(config, "IP_CPU");
    log_info(logger, "La IP de CPU es : %s", ip_cpu);

    //Inicia conexion con puerto dispatch de CPU
    char* puerto_cpu_dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    log_info(logger, "El PUERTO de CPU DISPATCH es : %s", puerto_cpu_dispatch);
    int socket_cpu_dispatch = crear_conexion(ip_cpu, puerto_cpu_dispatch);
    enviar_mensaje("Me conecto desde Kernel a PUERTO_CPU_DISPATCH", socket_cpu_dispatch);

    //Hilo de escucha con CPU puerto DISPATCH
    pthread_t hilo_atencion_cpu_dispatch;
    int *socket_cliente_hilo = (int)malloc(sizeof(int));
    socket_cliente_hilo = socket_cpu_dispatch;
    pthread_create(&hilo_atencion_cpu_dispatch,
                    NULL,
                    (void*) atender_cliente_kernel,
                    socket_cliente_hilo);
    pthread_detach(hilo_atencion_cpu_dispatch);

    //Inicia conexion con puerto interrupt de CPU
    char* puerto_cpu_interrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    log_info(logger, "El PUERTO de CPU INTERRUPT es : %s", puerto_cpu_interrupt);
    int socket_cpu_interrupt = crear_conexion(ip_cpu, puerto_cpu_interrupt);
    enviar_mensaje("Me conecto desde Kernel a PUERTO_CPU_INTERRUPT", socket_cpu_interrupt);
    
    
    lanzar_consola(quantum, socket_cliente_memoria, socket_cpu_dispatch, config);
    
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