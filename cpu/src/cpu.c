#include <stdlib.h>
#include <stdio.h>
#include <cpu.h>


#define CONFIG_FILE_PATH "./cfg/cpu.config"
#define LOG_FILE_PATH "./cfg/cpu.log"
#define PUERTO_ESCUCHA "PUERTO_ESCUCHA"
#define PUERTO_MEMORIA "PUERTO_MEMORIA"
#define IP_MEMORIA "IP_MEMORIA"




int main(int argc, char* argv[]) {
    decir_hola("CPU");
       char *ip_cpu;
        char *puerto_cpu;

        t_log *logger = log_create("./cfg/cpu.log", "CPU", true, LOG_LEVEL_INFO);
        log_info(logger, "Soy el CPU");
        t_config_cpu *config_cpu;

        config_cpu= levantar_config_cpu();

        puerto_cpu= config_cpu->puerto_escucha;

        log_info(logger,"realice configuracion");

        int conexion;


        log_info(logger,"levanto servidor");
        int server_fd=iniciar_servidor(puerto_cpu,logger);
        log_info(logger, "servidor listo para recibir cliente");

        pthread_t hiloKernel; // creacion del hilo para kernel


        log_info(logger, "atiendo cliente");

        while(1){

            int cliente_fd= esperar_cliente(server_fd,logger); // enviamos el socket servidor
            log_info(logger,"tengo el cliente %d", cliente_fd);
            pthread_arg *arg = malloc(sizeof(pthread_arg));

            arg-> socket_fd = cliente_fd;
            arg-> logger= logger;

            pthread_create(&hiloKernel,
                           NULL,
                           (void*)atender_kernel,
                           (void*)arg );

        }
        
        log_destroy(logger);
    }



 //   ConfigTerminate();
  //  LogTerminate(logger);
  //  liberar_conexion(conexionGeneral);

    t_config_cpu* levantar_config_cpu (){
      t_config *config;
    config = config_create("./cfg/cpu.config");

    t_config_cpu* cpuConfig = malloc(sizeof(t_config_cpu));

    cpuConfig->puerto_escucha = buscarEnConfig(config, "PUERTO_ESCUCHA");
    cpuConfig->tam_memoria = buscarEnConfig(config, "TAM_MEMORIA");
    cpuConfig->tam_pagina= buscarEnConfig(config, "TAM_PAGINA");
    cpuConfig->path_instrucciones = buscarEnConfig(config, "PATH_INTRUCCIONES");
    cpuConfig->retardo_respuesta = buscarEnConfig(config, "RETARDO_RESPUESTA");
    

    return cpuConfig;

}





void paquete(int conexion)
{
    // Ahora toca lo divertido!
    char *leido;
    t_paquete *paquete = crear_paquete();

    // Leemos y esta vez agregamos las lineas al paquete

    while (1)
    {
        leido = readline("> ");
        if (!leido)
        {
            break;
        }
        if (!strcmp(leido, ""))
        { // cuando se ingresa una línea vacía se corta la ejecución de leer_consola
            free(leido);
            break;
        }

        agregar_a_paquete(paquete, leido, strlen(leido) + 1);

        free(leido);
    }

    enviar_paquete(paquete, conexion);
   
    eliminar_paquete(paquete);
}


void atender_kernel(void *arg)
{
    int cliente_fd = ((pthread_arg *)arg)->socket_fd;
    t_log *logger = ((pthread_arg *)arg)->logger;
   

    t_list *lista;
    while (1)
    {
        log_info(logger, "Cliente %d", cliente_fd);
        int cod_op = recibir_operacion(cliente_fd);
        switch (cod_op)
        {
        case MENSAJE:
            recibir_mensaje(cliente_fd, logger);
            close(cliente_fd);
            break;
        case PAQUETE:
            lista = recibir_paquete(cliente_fd);
            log_info(logger, "Me llegaron los siguientes valores:\n");
            list_iterate(lista, (void *)iterator);
            break;
        case -1:
            log_error(logger, "el cliente se desconecto. Terminando servidor");
            free(arg);
            return EXIT_FAILURE;
        default:
            log_warning(logger, "Operacion desconocida. No quieras meter la pata");
            break;
        }
    }
   return EXIT_SUCCESS;

}


