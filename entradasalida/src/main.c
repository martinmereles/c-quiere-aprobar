#include "main.h"

t_log * logger;

int main(int argc, char* argv[]) {

    t_config * config;

    logger = iniciar_logger("entradaSalida-log.log", "entradaSalida");
    log_info(logger, "Soy un log");


    config = iniciar_config(logger, "entradaSalida.config");
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
  
    
    //Cierre de log y config     
    cerrar_log_config (logger,config);     
    
    return EXIT_SUCCESS;
}
