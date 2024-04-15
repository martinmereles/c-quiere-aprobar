#include <main.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include <commons/config.h>
#include <commons/log.h>
#include <readline/readline.h>

t_log* logger;

int main(int argc, char* argv[]) {
    
   logger = iniciar_logger();

    log_info(logger, "Soy un log");
    
    t_config* config = iniciar_config(logger);

    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    char* valor = config_get_string_value(config, "VALOR");
    log_info(logger, ip_memoria);
    log_info(logger, puerto_memoria);

    leer_consola(logger);
    

    int socket_cliente = crear_conexion(ip_memoria,puerto_memoria);

    enviar_mensaje("hola",socket_cliente);

    paquete(socket_cliente);
    terminar_programa(socket_cliente,logger,config);
    return EXIT_SUCCESS;
}


t_log* iniciar_logger(void)
{
    t_log* logger_nuevo = log_create("tp0-log", "kernel", true, LOG_LEVEL_INFO);

	return logger_nuevo;
}

t_config* iniciar_config(t_log* logger)
{
	t_config* nuevo_config;

    nuevo_config = config_create("kernel.config");

    if (nuevo_config==NULL){
        log_info(logger, "No existe el archivo kernel.config");
        exit(EXIT_FAILURE);
    }

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* command;
    command = readline("> ");
    while(strcmp(command,"")){
    log_info(logger, command);
    free(command);
    command = readline("> ");
    }
    free(command);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
    char* command;
    command = readline("> ");
    while(strcmp(command,"")){
    log_info(logger, command);
    agregar_a_paquete(paquete,command,strlen(command)+1);
    free(command);
    command = readline("> ");
    }
    free(command);
    enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
    config_destroy(config);
    liberar_conexion(conexion);
}