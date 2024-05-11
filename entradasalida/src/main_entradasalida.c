#include "../include/main_entradasalida.h"

t_log * logger;

int main(int argc, char* argv[]) {

    t_config * config;
    logger = iniciar_logger("./cfg/entradaSalida-log.log", "entradaSalida");
    config = iniciar_config(logger, "./cfg/entradaSalida.config");
    
    //Inicia conexion con kernel
    char* ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char* puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
    log_info(logger, "La IP de Kernel es : %s", ip_kernel);
    log_info(logger, "El PUERTO de Kernel es : %s", puerto_kernel);
    int socket_cliente_kernel = crear_conexion(ip_kernel,puerto_kernel);
    enviar_mensaje("Me conecto desde IO",socket_cliente_kernel);
  
    //Inicia conexión con Memoria
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "La IP de Memoria es : %s", ip_memoria);
    log_info(logger, "El PUERTO de Memoria es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje("Me conecto desde IO",socket_cliente_memoria);
    
    //Cierre de log y config     
    cerrar_log_config (logger,config);     
    
    return EXIT_SUCCESS;
}
