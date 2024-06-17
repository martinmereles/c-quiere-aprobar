#include "../include/manage_clients.h"


void iniciar_hilo_server_kernel(char* puerto){
    int socket_servidor = iniciar_servidor(puerto);
    while (1) {
        pthread_t hiloAtencion;
        int *socket_cliente = (int)malloc(sizeof(int));
        socket_cliente = esperar_cliente(socket_servidor);
        pthread_create(&hiloAtencion,
                        NULL,
                        (void*) atender_cliente_kernel,
                        socket_cliente);
        pthread_detach(hiloAtencion);
    }
}

void atender_cliente_kernel(int socket_cliente){
	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(socket_cliente);
		switch (cod_op) {
		case MENSAJE:
            int size;
            char* buffer = recibir_buffer(&size, socket_cliente);
            log_info(logger, "Kernel recibio el mensaje %s", buffer);
            char ** mensaje_split = string_split(buffer, " ");
            if(strcmp(mensaje_split[0], "IO_GEN_SLEEP") == 0){
                io_gen_sleep(mensaje_split[1], mensaje_split[2], socket_cliente);
            }
            if(strcmp(mensaje_split[0], "CONECTAR_INTERFAZ") == 0){
                conectar_interfaz(mensaje_split[1], mensaje_split[2], socket_cliente);
            }
            if(strcmp(mensaje_split[0], "FIN_IO") == 0){
                liberar_interfaz(mensaje_split[1]);
            }
            free(buffer);
			break;
		case PAQUETE:
            //TO DO: Guardar pcb lista
            int size2;
            pcb_t * pcb = malloc (sizeof(pcb_t));
	        pcb->reg_generales = malloc(sizeof(registros_t));
            pcb = recibir_buffer_pcb (&size2, socket_cliente);
            printf ("El PID es: %d \n", pcb->pid);
            free (pcb);
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

void conectar_interfaz (char* tipo_interfaz, char* identificador, int socket_interfaz){
    t_interfaz* interfaz = malloc(sizeof(t_interfaz));

    interfaz->socket = socket_interfaz;
    interfaz->tipo_interfaz = tipo_interfaz;
    interfaz->identificador = identificador;
    sem_init(&interfaz->sem_uso, 0, 1);

    list_add (INTERFACES, interfaz);
}

bool es_interfaz_buscada(char* identificador, void* elemento){
    t_interfaz* aux = malloc(sizeof(t_interfaz));
    aux = elemento;
    bool aux2 = (strcmp(aux->identificador, identificador) == 0);
    return (aux2);
}

void io_gen_sleep(char * interfaz, char* unidad_tiempo, int socket_cliente){
    
    bool _es_interfaz_buscada(void* elemento){
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz* interfaz_encontrada =  malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);
    int socket_interfaz = interfaz_encontrada->socket;
    
    sem_wait(&interfaz_encontrada->sem_uso);

    char* mensaje = string_new();
    string_append(&mensaje, "IO_GEN_SLEEP ");
    string_append(&mensaje, unidad_tiempo);

    log_info(logger, "Se envio el mensaje a %s", interfaz);
    enviar_mensaje(mensaje, socket_interfaz);

    
}

void liberar_interfaz(char * interfaz){

    bool _es_interfaz_buscada(void* elemento){
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz* interfaz_encontrada =  malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);

    log_info(logger, "Kernel recibio la finalizacion de %s", interfaz);
    sem_post(&interfaz_encontrada->sem_uso);
    
}
