#include "../include/main_entradasalida.h"

t_log * logger;

int main(int argc, char* argv[]) {

    t_config * config;

    logger = iniciar_logger("./cfg/entradaSalida-log.log", "entradaSalida");
    log_info(logger, "Soy un log");


    config = iniciar_config(logger, "./cfg/entradaSalida.config");
    char* valor = config_get_string_value(config, "TIPO_INTERFAZ");
    leer_consola(logger);
    
    //Inicia conexion con kernel
    char* ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char* puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
    log_info(logger, "La IP de Kernel es : %s", ip_kernel);
    log_info(logger, "El PUERTO de Kernel es : %s", puerto_kernel);
    int socket_cliente_kernel = crear_conexion(ip_kernel,puerto_kernel);
    enviar_mensaje(valor,socket_cliente_kernel);
    paquete(socket_cliente_kernel);
    liberar_conexion(socket_cliente_kernel);
  

  //Inicia conexion con memoria
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "La IP de Memoria es : %s", ip_memoria);
    log_info(logger, "El PUERTO de Memoria es : %s", puerto_memoria);
    int socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje(valor,socket_cliente_memoria);
    paquete(socket_cliente_memoria);
    liberar_conexion(socket_cliente_memoria);
  
    
    //Cierre de log y config     
    cerrar_log_config (logger,config);     
    
    return EXIT_SUCCESS;
}
