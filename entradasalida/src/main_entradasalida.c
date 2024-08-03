#include "../include/main_entradasalida.h"

t_log *logger;
char *nombre_interfaz;
char *tipo_interfaz;
int socket_cliente_memoria;
t_bitarray *bitmap_bloques_libres;
sem_t sem_fs;

int main(int argc, char *argv[])
{

    char *nombre;
    nombre = readline("Ingrese el nombre de la interfaz > ");
    while (string_contains(nombre, " "))
    {
        log_info(logger, "No puede contener espacios");
        free(nombre);
        nombre = readline("Ingrese el nombre de la interfaz > ");
    }
    nombre_interfaz = malloc(sizeof(nombre));
    strcpy(nombre_interfaz, nombre);
    
    free(nombre);

    char *path_log = string_new();
    char *path_config = string_new();
    string_append(&path_log, "./");
    string_append(&path_log, nombre_interfaz);
    string_append(&path_log, ".log");
    string_append(&path_config, "./");
    string_append(&path_config, nombre_interfaz);
    string_append(&path_config, ".config");

    t_config *config;
    logger = iniciar_logger(path_log, nombre_interfaz);
    config = iniciar_config(logger, path_config);
    tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
    char *tiempo_unidad_trabajo = config_get_string_value(config, "TIEMPO_UNIDAD_TRABAJO");
    log_info(logger, "Se ingreso como nombre de la interfaz: %s", nombre_interfaz);
    if (strcmp(tipo_interfaz, "DIALFS") == 0)
    {   
        sem_init(&sem_fs, 0, 1);
        iniciar_dialfs(config);
        
    }

    // Inicia conexion con kernel
    pthread_t hiloKernel;
    pthread_create(&hiloKernel,
                   NULL,
                   iniciar_hilo_kernel,
                   config);

    // Inicia conexión con Memoria
    if (!strcmp(tipo_interfaz, "GENERICA") == 0)
    { // Si es disntinto a generica va por aca
        char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
        char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
        log_info(logger, "La IP de Memoria es : %s", ip_memoria);
        log_info(logger, "El PUERTO de Memoria es : %s", puerto_memoria);
        socket_cliente_memoria = crear_conexion(ip_memoria, puerto_memoria);
        enviar_mensaje("Me conecto desde IO", socket_cliente_memoria);
    }

    // Esperar al hilo kernel
    pthread_join(hiloKernel, NULL);

    // Cierre de log y config
    cerrar_log_config(logger, config);
    free(nombre_interfaz);

    return EXIT_SUCCESS;
}