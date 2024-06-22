#include "../include/auxiliares.h"

void iniciar_hilo_kernel(t_config* config){
    char* ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char* puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
    char* tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
    char* tiempo_unidad_trabajo_string = config_get_string_value(config, "TIEMPO_UNIDAD_TRABAJO");
    int tiempo_unidad_trabajo = atoi(tiempo_unidad_trabajo_string);
    log_info(logger, "La IP de Kernel es : %s", ip_kernel);
    log_info(logger, "El PUERTO de Kernel es : %s", puerto_kernel);
    char* nombre;
    nombre = readline("Ingrese el nombre de la interfaz > ");
    while(string_contains(nombre, " ")){
        log_info(logger, "No puede contener espacios");
    	free(nombre);
    	nombre = readline("Ingrese el nombre de la interfaz > ");
    }
    nombre_interfaz = malloc(sizeof(nombre));
    strcpy(nombre_interfaz, nombre);
    log_info(logger, "Se ingreso como nombre de la interfaz: %s", nombre_interfaz);
    
    int socket_cliente_kernel = crear_conexion(ip_kernel,puerto_kernel);
    char* msj_conexion = string_new();
    string_append(&msj_conexion, "CONECTAR_INTERFAZ ");
    string_append(&msj_conexion, tipo_interfaz);
    string_append(&msj_conexion, " ");
    string_append(&msj_conexion, nombre);
    enviar_mensaje(msj_conexion,socket_cliente_kernel);
    free(nombre);

    t_list* lista;
    while (1) {
        int cod_op = recibir_operacion(socket_cliente_kernel);; 
        switch (cod_op) {
        case MENSAJE:
            int size;
            char* buffer = recibir_buffer(&size, socket_cliente_kernel);
            log_info(logger, "Me llego el mensaje %s", buffer);
            entender_mensajes(buffer,socket_cliente_kernel, tiempo_unidad_trabajo);
            free(buffer);
            break;
        case PAQUETE:
            lista = recibir_paquete(socket_cliente_kernel);
            log_info(logger, "Me llegaron los siguientes valores:\n");
            list_iterate(lista, (void*) iterator);
            break;
        case -1:
            log_error(logger, "el cliente se desconecto.");
            return EXIT_FAILURE;
        default:
            log_warning(logger,"Operacion desconocida. No quieras meter la pata");
            break;
        }
    }

}

void entender_mensajes(char* mensaje, int socket_cliente,int tiempo_unidad_trabajo){
    char ** mensaje_split = string_split(mensaje, " ");
    if(strcmp(mensaje_split[0], "IO_GEN_SLEEP") == 0){
        io_gen_sleep(mensaje_split[1], tiempo_unidad_trabajo, socket_cliente);
    }
}

void io_gen_sleep(char* unidades_tiempo, int tiempo_unidad_trabajo, int socket_cliente){
    int unidades_tiempo_int = atoi(unidades_tiempo);
    log_info(logger, "Se inicia tarea IO_GEN_SLEEP");
    usleep((tiempo_unidad_trabajo*unidades_tiempo_int)*1000);
    char* mensaje = string_new();
    string_append(&mensaje, "FIN_IO ");
    string_append(&mensaje, nombre_interfaz);
    log_info(logger, "Se envia el mensaje=> %s", mensaje);
    enviar_mensaje(mensaje, socket_cliente);
    log_info(logger, "Se termino tarea IO_GEN_SLEEP");
}