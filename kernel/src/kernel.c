
#include "conexion.h"
#include "typeDef.h"
#include "shared_utils.h"
#include "server.h"
#include "kernel.h"


int main(int argc, char* argv[]) {
    decir_hola("Kernel");
   int conexion_cpu;
    int conexion_memoria;
    int conexion_filesystem;
        

        t_log *logger = log_create("./cfg/kernel.log", "kernel", true, LOG_LEVEL_INFO);
        log_info(logger, "Soy el Kernel!");


        t_kernel_config* kernel_config;
        kernel_config = levantar_config_kernel();
        log_info(logger, "el puerto de escucha es %s", kernel_config->puerto_escucha);    

        log_info(logger, "levante la configuracion");
        log_info(logger, "conecto a CPU");
    
        conexion_cpu = crear_conexion(kernel_config->ip_cpu, kernel_config-> puerto_cpu_interrupt);
        log_info(logger, "cree conexion a CPU");
    
        enviar_mensaje("me conecte a CPU", conexion_cpu);

        log_info(logger, "me conecte y mande un mensaje");
        log_info(logger, "levanto el servidor");
        int server_fd = iniciar_servidor(kernel_config->puerto_escucha, logger);
        log_info(logger, "Servidor listo para recibir al cliente");

        pthread_t hiloConsolas;

        log_info(logger, "atiendo clientes");
        while (1)
        {
            int cliente_fd = esperar_cliente(server_fd, logger);
            log_info(logger, "Tengo el cliente %d", cliente_fd);
            pthread_arg *arg = malloc(sizeof(pthread_arg));

            arg->socket_fd = cliente_fd;
            arg->logger = logger;

            pthread_create(&hiloConsolas, // Crea hilo que crea hilos atendedores de clientes
                           NULL,
                           (void *)atender_cliente,
                           (void *)arg);
            // pthread_join(hiloConsolas, NULL);
        }

        log_destroy(logger);
        
    }


t_list *recibir_instrucciones(int socket_cliente, t_log* logger)
{

    log_info(logger, "--RECIBIENDO INSTRUCCIONES--");

    int cant;
    recv(socket_cliente, &cant, sizeof(int), 0);

    log_info(logger, "---Cantidad de Instrucciones: %d--\n", cant);

    enum op_code_consola operacion;

    recv(socket_cliente, &operacion, sizeof(operacion), 0);

    log_info(logger, "---op_code: %d--\n", operacion);

    t_list *instr = list_create();

    for (int i = 0; i < cant; i++)
    {
        int buffer_size;
        recv(socket_cliente, &buffer_size, sizeof(buffer_size), 0);

        log_info(logger, "---Longitud:: %d", buffer_size);

        char *buffer = malloc(buffer_size);
        recv(socket_cliente, buffer, buffer_size, 0);

        log_info(logger, "---Instruccion: %s", buffer);

        list_add(instr, buffer);
    }
    return instr;
}

void atencion_consola(void *arg)
{
    int cliente_fd = ((pthread_arg *)arg)->socket_fd;
    t_log *logger = ((pthread_arg *)arg)->logger;

    t_list *listaInstrucciones = list_create();
    t_list *tablaSegmentos = list_create();
    list_add_all(listaInstrucciones, recibir_instrucciones(cliente_fd, logger));
    close(cliente_fd);
    return 0;

}



void atender_cliente(void *arg)
{
    int cliente_fd = ((pthread_arg *)arg)->socket_fd;
    t_log *logger = ((pthread_arg *)arg)->logger;


    t_list *lista;
    while (1)
    {
        log_info(logger, "Cliente %d", cliente_fd);
        int op_code = recibir_operacion(cliente_fd);
        enum op_code code;
        code = op_code;
        switch (code)
        {
        /*case MENSAJE:
            recibir_mensaje(cliente_fd, logger);
            close(cliente_fd);
            break;*/
        case PAQUETE:
            lista = recibir_paquete(cliente_fd);
            log_info(logger, "Me llegaron los siguientes valores:\n");
            list_iterate(lista, (void *)iterator);
            break;
        case INSTR:
            log_info(logger, "voy a usar la funcion atencion_consola");
            //atencion_consola (arg);
            t_list *listaInstrucciones = list_create();
            //t_list *tablaSegmentos = list_create();
            list_add_all(listaInstrucciones, recibir_instrucciones(cliente_fd, logger));
            close(cliente_fd);
            log_info(logger, "use la funcion atencion_consola");

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

t_kernel_config* levantar_config_kernel () 
{
    t_config *config;
    config = config_create("./cfg/kernel.config");

    t_kernel_config* kernelConfig = malloc(sizeof(t_kernel_config));

    kernelConfig->ip_memoria = buscarEnConfig(config, "IP_MEMORIA");
    kernelConfig->puerto_memoria = buscarEnConfig(config, "PUERTO_MEMORIA");
    kernelConfig->puerto_cpu_dispatch= buscarEnConfig(config, "IP_FILESYSTEM");
    kernelConfig->ip_cpu = buscarEnConfig(config, "IP_CPU");
    kernelConfig->puerto_escucha = buscarEnConfig(config, "PUERTO_ESCUCHA");
    kernelConfig->algoritmo_planificacion = buscarEnConfig(config, "ALGORITMO_PLANIFICACION");
    kernelConfig->puerto_cpu_interrupt = buscarEnConfig(config, "PUERTO_CPU_INTERRUPT");
    kernelConfig->quantum= buscarEnConfig(config, "QUANTUM");
    kernelConfig->grado_multiprogramacion = buscarEnConfig(config, "GRADO_MULTIPROGRAMACION");
    kernelConfig->recursos = buscarEnConfig(config, "RECURSOS");
    kernelConfig->instancias_recursos = buscarEnConfig(config, "INSTANCIAS_RECURSOS");
    

    return kernelConfig;

}

void atender_cpu(void *arg) // cliente cpu 
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




