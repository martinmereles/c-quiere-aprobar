#include "../include/auxiliares.h"

void iniciar_hilo_server_memoria(char* puerto){
    int socket_servidor = iniciar_servidor(puerto);
    while (1) {
        pthread_t hiloAtencion;
        int *socket_cliente = (int)malloc(sizeof(int));
        socket_cliente = esperar_cliente(socket_servidor);
        pthread_create(&hiloAtencion,
                        NULL,
                        (void*) atender_cliente_memoria,
                        socket_cliente);
        pthread_detach(hiloAtencion);
    }
}

void atender_cliente_memoria(int socket_cliente){
	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(socket_cliente);; 
		switch (cod_op) {
		case MENSAJE:
            int size;
            char* buffer = recibir_buffer(&size, socket_cliente);
            log_info(logger, "Me llego el mensaje %s", buffer);
            char ** mensaje_split = string_split(buffer, " ");
            if(strcmp(mensaje_split[0], "INICIAR_PROCESO") == 0){
                iniciar_proceso(mensaje_split[1], mensaje_split[2]);
            }
            if(strcmp(mensaje_split[0], "PROXIMA_INSTRUCCION") == 0){
                proxima_instruccion(mensaje_split[1], mensaje_split[2], socket_cliente);
            }
            free(buffer);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente );
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto.");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}

void iniciar_proceso(char* process_id, char* path){
    int process = atoi(process_id);
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    t_instruccion_memoria* instruccion = malloc(sizeof(t_instruccion_memoria));
    instruccion->lista_instrucciones = list_create();
    instruccion->process_id = process;

    fseek(f, 0L, SEEK_END);
    int size_file = ftell(f);
    rewind(f);
    char* texto_completo = malloc(size_file);
    fread(texto_completo,sizeof(char),size_file,f);
    char ** instrucciones_split = string_split(texto_completo, "\n");

    int i = 0;
    int length = string_array_size(instrucciones_split);
    while(i<length){
        list_add(instruccion->lista_instrucciones, instrucciones_split[i]);
        log_info(logger, "Instrucción cargada=> %s - Proceso %d",instrucciones_split[i], process);
        i++;
    }

    list_add(memoria_instrucciones, instruccion);
    fclose(f);
    
}

void proxima_instruccion(char* process_id_find, char* program_counter, int socket_cliente){

    t_instruccion_memoria* proceso = malloc(sizeof(t_instruccion_memoria));
    proceso->lista_instrucciones = list_create();
    int i = 0;
    int pid = 0;
    bool encontrado =false;
    while(!encontrado && list_size(memoria_instrucciones)>i){
        proceso = list_get(memoria_instrucciones,i);
        pid = proceso->process_id;
        if(pid == atoi(process_id_find)) {encontrado=true;} else {i++;}
    }
    if(!encontrado){
        log_error(logger, "No existe el PID %s", process_id_find);
    }else{
        int pc = atoi(program_counter);
        char* instruccion = list_get(proceso->lista_instrucciones,pc);
        log_info(logger, "Instruccion a devolver=> %s",instruccion);
        log_info(logger, "Retardo %d ",retardo_respuesta_sec);
        sleep(retardo_respuesta_sec);
        enviar_mensaje(instruccion,socket_cliente);
    }
}

