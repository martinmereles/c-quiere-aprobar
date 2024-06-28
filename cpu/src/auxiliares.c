#include "../include/auxiliares.h" 

void atender_cliente_cpu(int socket_cliente){
	t_list* lista;
        int cod_op = recibir_operacion(socket_cliente);; 
        switch (cod_op) {
        case MENSAJE:
            int size;
            char* buffer = recibir_buffer(&size, socket_cliente);
            log_info(logger, "Me llego el mensaje %s", buffer);
            char ** mensaje_split = string_split(buffer, " ");
            if (es_intruccion(mensaje_split[0]))
            {
            realloc(instruccion_exec,&size);
            strcpy(instruccion_exec,buffer);
            }
            free(buffer);
            break;
        case PAQUETE:
            lista = recibir_paquete(socket_cliente );
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

bool es_intruccion (char* palabra) {

    if (
    strcmp(palabra,"SET")==0
    ||strcmp(palabra,"MOV_IN")==0
    ||strcmp(palabra,"MOV_OUT")==0
    ||strcmp(palabra,"SUM")==0
    ||strcmp(palabra,"SUB")==0
    ||strcmp(palabra,"JNZ")==0
    ||strcmp(palabra,"RESIZE")==0
    ||strcmp(palabra,"COPY_STRING")==0
    ||strcmp(palabra,"WAIT")==0
    ||strcmp(palabra,"SIGNAL")==0
    ||strcmp(palabra,"IO_GEN_SLEEP")==0
    ||strcmp(palabra,"IO_STDIN_READ")==0
    ||strcmp(palabra,"IO_STDOUT_WRITE")==0
    ||strcmp(palabra,"IO_FS_CREATE")==0
    ||strcmp(palabra,"IO_FS_DELETE")==0
    ||strcmp(palabra,"IO_FS_TRUNCATE")==0
    ||strcmp(palabra,"IO_FS_WRITE")==0
    ||strcmp(palabra,"IO_FS_READ")==0
    ||strcmp(palabra,"EXIT")==0)
    {
        return true;
    }
    else {
        return false;
    }
}

void atender_cliente_dispatch(int socket_servidor_dispatch){
    t_list* lista;
	log_info(logger, "El socket_servidor_dispatch es : %d", socket_servidor_dispatch);
	socket_kernel_dispatch = esperar_cliente(socket_servidor_dispatch);
	while (1) {
        log_info(logger, "El SOCKET DISPATCH cliente es : %d", socket_kernel_dispatch);
		int cod_op = recibir_operacion(socket_kernel_dispatch);
        
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_kernel_dispatch);
			break;
		case PAQUETE:
			int size2;
            pcb_t * pcb = malloc (sizeof(pcb_t));
	        pcb->reg_generales = malloc(sizeof(registros_t));
            pcb = recibir_buffer_pcb (&size2, socket_kernel_dispatch);
            reg = pcb->reg_generales;
            contexto = pcb;
            log_info(logger, "Se recibió el PCB con PID %d", contexto->pid);
            //Iniciamos temporizador
            temporizador = temporal_create();
            sem_post(&sem_execute);
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

void atender_cliente_interrupt(int socket_servidor_interrupt){
    t_list* lista;
	log_info(logger, "El socket_servidor_interrupt es : %d", socket_servidor_interrupt);
	int socket_kernel_interrupt = esperar_cliente(socket_servidor_interrupt);
	while (1) {
        log_info(logger, "El SOCKET INTERRUPT cliente es : %d", socket_kernel_interrupt);
		int cod_op = recibir_operacion(socket_kernel_interrupt);
        
		switch (cod_op) {
		case MENSAJE:
            int size;
            char* buffer = recibir_buffer(&size, socket_kernel_interrupt);
            log_info(logger, "CPU recibio el mensaje por el puerto Interrupt %s", buffer);
            char ** buffer_split = string_split(buffer, " ");
            if(!strcmp(buffer_split[0],"EXIT") || !strcmp(buffer_split[0],"FIN_QUANTUM")){
                char* duplicado = string_new();
                duplicado = string_duplicate(buffer);
                list_add(INTERRUPCIONES, duplicado);
            }
            free(buffer);
            
			break;
		case PAQUETE:
            //En principio, interrupt no recibe paquetes
			break;
		case -1:
			log_error(logger, "El cliente se desconecto.");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
    liberar_conexion(socket_kernel_interrupt);
}
