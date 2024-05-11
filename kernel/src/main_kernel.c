#include "../include/main_kernel.h"

t_log* logger;

int main(int argc, char* argv[]) {
    
    logger = iniciar_logger("./cfg/kernel-log.log", "kernel");

    log_info(logger, "Soy un log");
    
    t_config* config = iniciar_config(logger, "./cfg/kernel.config");

    char* valor = config_get_string_value(config, "CLAVE");
 leer_consola(logger);
    
    /* //Inicia conexion con memoria
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
 comentado para prueba de hilos*/
    //Inicia escucha en servidor
	char* puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
    int socket_servidor = iniciar_servidor(puerto);
 //   int socket_cliente = esperar_cliente(socket_servidor);
    pthread_t hiloCliente;

        while (1)
        {
            int cliente_fd = esperar_cliente(socket_servidor);
            log_info(logger, "Tengo el cliente %d", cliente_fd);
            pthread_arg *arg = malloc(sizeof(pthread_arg));

            arg->socket_fd = cliente_fd;

            pthread_create(&hiloCliente, // Crea hilo que crea hilos atendedores de consolas
                           NULL,
                           (void *)atender_clientes,
                           (void *)arg);
            // pthread_join(hiloConsolas, NULL);
            
        }
        /*
    t_list* lista;
	    while (1) {
	    	int cod_op = recibir_operacion(socket_cliente);
	    	switch (cod_op) {
	    	case MENSAJE:
	    		recibir_mensaje(socket_cliente);
	    		break;
	    	case PAQUETE:
	    		lista = recibir_paquete(socket_cliente );
	    		log_info(logger, "Me llegaron los siguientes valores:\n");
	    		list_iterate(lista, (void*) iterator);
	    		break;
	    	case -1:
	    		log_error(logger, "el cliente se desconecto. Terminando servidor");
	    		return EXIT_FAILURE;
	    	default:
	    		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
	    		break;
	    	}
	    }
    */

    //Cierre de log y config
    cerrar_log_config (logger,config); 
    
    return EXIT_SUCCESS;
}

void iterator(char* value) 
{
	log_info(logger,"%s", value);
}

void atender_clientes(void *arg)
{
    while (1)
        {
            int cliente_fd = esperar_cliente(socket_servidor);
            log_info(logger, "Tengo el cliente %d", cliente_fd);   int socket_cliente = ((pthread_arg *)arg)->socket_fd;
    t_list* lista;
    while (1)
    {
        log_info(logger, "Cliente %d", socket_cliente);

	    	int cod_op = recibir_operacion(socket_cliente);
	    	switch (cod_op) {
	    	case MENSAJE:
	    		recibir_mensaje(socket_cliente);
	    		break;
	    	case PAQUETE:
	    		lista = recibir_paquete(socket_cliente );
	    		log_info(logger, "Me llegaron los siguientes valores:\n");
	    		list_iterate(lista, (void*) iterator);
	    		break;
	    	case -1:
	    		log_error(logger, "el cliente se desconecto. Terminando servidor");
	    		return EXIT_FAILURE;
	    	default:
	    		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
	    		break;
	    	}
    }
}