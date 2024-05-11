#include "../include/main_kernel.h"

t_log* logger;

int main(int argc, char* argv[]) {
    
    logger = iniciar_logger("./cfg/kernel-log.log", "kernel");
    t_config* config = iniciar_config(logger, "./cfg/kernel.config");

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


    pthread_join(hiloServidor, NULL);
    //Cierre de log y config
    cerrar_log_config (logger,config); 
    
    return EXIT_SUCCESS;
}