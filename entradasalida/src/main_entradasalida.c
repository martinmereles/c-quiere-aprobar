#include "../include/main_entradasalida.h"

t_log * logger;
char* nombre_interfaz;
char* tipo_interfaz;
int socket_cliente_memoria;
t_bitarray* bitmap_bloques_libres;

int main(int argc, char* argv[]) {

    t_config * config;
    logger = iniciar_logger("./cfg/entradaSalida-log.log", "entradaSalida");
    config = iniciar_config(logger, "./cfg/entradaSalida.config");
    tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
    char* tiempo_unidad_trabajo = config_get_string_value(config, "TIEMPO_UNIDAD_TRABAJO");

    if(strcmp(tipo_interfaz, "DIALFS") == 0){
        iniciar_dialfs(config);
        for(int i = 0; i<bitmap_bloques_libres->size;i++){
            if(bitarray_test_bit(bitmap_bloques_libres,i)){
                printf("%d-%s\n", i, "True");
            }else{
                printf("%d-%s\n", i, "False");
            }
        }
        return EXIT_SUCCESS;
    }

    //Inicia conexion con kernel
    pthread_t hiloKernel;
	pthread_create(&hiloKernel,
                        NULL,
                        iniciar_hilo_kernel,
                        config);
	
  
    //Inicia conexión con Memoria
    if(!strcmp(tipo_interfaz, "GENERICA") == 0){ //Si es disntinto a generica va por aca
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    log_info(logger, "La IP de Memoria es : %s", ip_memoria);
    log_info(logger, "El PUERTO de Memoria es : %s", puerto_memoria);
    socket_cliente_memoria = crear_conexion(ip_memoria,puerto_memoria);
    enviar_mensaje("Me conecto desde IO",socket_cliente_memoria);
    }


    //Esperar al hilo kernel
    pthread_join(hiloKernel, NULL);

    //Cierre de log y config     
    cerrar_log_config (logger,config);   
    free(nombre_interfaz);  
    
    return EXIT_SUCCESS;
}



