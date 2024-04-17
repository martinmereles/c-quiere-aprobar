#include <main.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils/client-server.h>
#include <commons/config.h>
#include <commons/log.h>
#include <readline/readline.h>

t_log* logger;

int main(int argc, char* argv[]) {
    
   logger = iniciar_logger("tp0-kernel-log.log", "kernel");

    log_info(logger, "Soy un log");
    
    t_config* config = iniciar_config(logger, "kernel.config");

    char* valor = config_get_string_value(config, "CLAVE");
    leer_consola(logger);
    
    //Inicia conexion con memoria
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "La IP de MEMORIA es : %s", ip_memoria);
    log_info(logger, "El PUERTO de MEMORIA es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje(valor,socket_cliente_memoria);
    paquete(socket_cliente_memoria);
    liberar_conexion(socket_cliente_memoria);

    //Inicia conexion con cpu
    char* ip_cpu = config_get_string_value(config, "IP_CPU");
    char* puerto_cpu = config_get_string_value(config, "PUERTO_CPU");
    log_info(logger, "La IP de CPU es : %s", ip_cpu);
    log_info(logger, "El PUERTO de CPU es : %s", puerto_cpu);
    int socket_cliente_cpu = crear_conexion(ip_cpu,puerto_cpu);
    enviar_mensaje(valor,socket_cliente_cpu);
    paquete(socket_cliente_cpu);
    liberar_conexion(socket_cliente_cpu);
    

    //Cierre de log y config
    log_destroy(logger);
    config_destroy(config);
    
    return EXIT_SUCCESS;
}

void iterator(char* value) 
{
	log_info(logger,"%s", value);
}