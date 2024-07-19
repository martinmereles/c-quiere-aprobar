#include "../include/main_entradasalida.h"

t_log * logger;
char* nombre_interfaz;
char* tipo_interfaz;
int socket_cliente_memoria;
t_bitarray* bitmap_bloques_libres;
sem_t sem_fs;

int main(int argc, char* argv[]) {

    t_config * config;
    logger = iniciar_logger("./cfg/entradaSalida-log.log", "entradaSalida");
    config = iniciar_config(logger, "./cfg/entradaSalida.config");
    tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
    char* tiempo_unidad_trabajo = config_get_string_value(config, "TIEMPO_UNIDAD_TRABAJO");

    if(strcmp(tipo_interfaz, "DIALFS") == 0){
        iniciar_dialfs(config);
        sem_init(&sem_fs, 0, 1);
    }
    //ARRANCA PRUEBA
    for(int i = 0; i<100;i++){
        if(bitarray_test_bit(bitmap_bloques_libres,i)){
            printf("%d - Ocupado\n", i);
        }else{
            printf("%d - Libre\n", i);
        }
    }

    io_fs_create("Prueba45.txt", config);

    for(int i = 0; i<100;i++){
        if(bitarray_test_bit(bitmap_bloques_libres,i)){
            printf("%d - Ocupado\n", i);
        }else{
            printf("%d - Libre\n", i);
        }
    }

    io_fs_truncate("Prueba45.txt", 400, config);

    for(int i = 0; i<100;i++){
        if(bitarray_test_bit(bitmap_bloques_libres,i)){
            printf("%d - Ocupado\n", i);
        }else{
            printf("%d - Libre\n", i);
        }
    }

    io_fs_delete("Prueba45.txt", config);

    for(int i = 0; i<100;i++){
        if(bitarray_test_bit(bitmap_bloques_libres,i)){
            printf("%d - Ocupado\n", i);
        }else{
            printf("%d - Libre\n", i);
        }
    }

    return 0;
    //TERMINA PRUEBA

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



