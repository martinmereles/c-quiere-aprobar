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
    t_config* config = iniciar_config(logger, "./cfg/kernel.config");
    char* algoritmo = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
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
                io_gen_sleep(mensaje_split[1], mensaje_split[2], mensaje_split[3]);
            }
            if(strcmp(mensaje_split[0], "CONECTAR_INTERFAZ") == 0){
                conectar_interfaz(mensaje_split[1], mensaje_split[2], socket_cliente);
            }
            if(strcmp(mensaje_split[0], "FIN_IO") == 0){
                liberar_interfaz(mensaje_split[1], mensaje_split[2], algoritmo);
            }
            free(buffer);
			break;
		case PAQUETE:
            //TO DO: Guardar pcb lista
            int size2;

            pcb_motivo_t* pcb_deserealizado = malloc(sizeof(pcb_motivo_t));
            pcb_deserealizado->pcb = malloc (sizeof(pcb_t));
            pcb_deserealizado->pcb->reg_generales = malloc(sizeof(registros_t));
            pcb_deserealizado->motivo = string_new();

            pcb_deserealizado = recibir_buffer_pcb_motivo(&size2, socket_cliente);

            if(strcmp(pcb_deserealizado->motivo, "INTERRUPTED_BY_USER") == 0){
                //agregar el pcb a TERMINATED
                sem_wait(&sem_array_estados[4].mutex);
                list_add(QUEUE_TERMINATED, pcb_deserealizado->pcb);
                sem_post(&sem_array_estados[4].mutex);
                sem_post(&sem_array_estados[4].contador);

                char* mensaje = string_new();
                string_append(&mensaje, "EXIT ");
                string_append(&mensaje, string_itoa(pcb_deserealizado->pcb->pid));
                enviar_mensaje(mensaje, socket_memoria);

                sem_wait(&sem_array_estados[2].contador);
                sem_post(&sem_grado_multiprog);
                sem_post(&sem_multiprocesamiento);
            }
            if(strcmp(pcb_deserealizado->motivo, "FIN_QUANTUM") == 0){
                //agregar el pcb a READY 
                sem_wait(&sem_array_estados[2].mutex);
                list_remove(QUEUE_RUNNING, 0);
                sem_post(&sem_array_estados[2].mutex);
                sem_wait(&sem_array_estados[2].contador);

                sem_wait(&sem_array_estados[1].mutex);
                list_add(QUEUE_READY, pcb_deserealizado->pcb);
                sem_post(&sem_array_estados[1].mutex);
                sem_post(&sem_array_estados[1].contador);

                sem_post(&sem_multiprocesamiento);
            }
            if(strcmp(pcb_deserealizado->motivo, "IO") == 0){

                sem_wait(&sem_array_estados[2].mutex);
                list_remove(QUEUE_RUNNING, 0);
                sem_post(&sem_array_estados[2].mutex);
                sem_wait(&sem_array_estados[2].contador);
                sem_wait(&sem_array_estados[3].mutex);
                list_add(QUEUE_BLOCKED, pcb_deserealizado->pcb);
                sem_post(&sem_array_estados[3].mutex);
                sem_post(&sem_array_estados[3].contador);    
                sem_post(&sem_multiprocesamiento);
            }
            

            
            //free (pcb);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto.");
            desconectar_interfaz(socket_cliente);
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
    interfaz->queue_instrucciones = list_create();
    sem_init(&interfaz->sem_uso, 0, 1);
    sem_init(&interfaz->contador, 0, 0);

    pthread_t hilo_Interfaz;
	pthread_create(&hilo_Interfaz,
                        NULL,
                        consumidor_interfaz,
                        interfaz);
	pthread_detach(hilo_Interfaz);

    interfaz->id = hilo_Interfaz;
    sem_wait(&mutex_lista_interfaces);
    list_add (INTERFACES, interfaz);
    sem_post(&mutex_lista_interfaces);
}

void consumidor_interfaz(t_interfaz* interfaz){
    while(1){
        sem_wait(&interfaz->sem_uso);
        sem_wait(&interfaz->contador);
        int socket_interfaz = interfaz->socket;
        log_info(logger, "Se envio el mensaje a %s", interfaz->identificador);
        sem_wait(&mutex_lista_interfaces);
        char * mensaje = list_remove (interfaz->queue_instrucciones, 0);
        sem_post(&mutex_lista_interfaces);
        enviar_mensaje(mensaje, socket_interfaz);
    }
}

bool es_interfaz_buscada(char* identificador, void* elemento){
    t_interfaz* aux = malloc(sizeof(t_interfaz));
    aux = elemento;
    bool aux2 = (strcmp(aux->identificador, identificador) == 0);
    return (aux2);
}

bool es_pcb_buscado(int pid_buscado, void* elemento){
    pcb_t* aux = malloc(sizeof(pcb_t));
    aux = elemento;
    bool aux2 = (aux->pid == pid_buscado);
    return (aux2);
}

void io_gen_sleep(char * interfaz, char* unidad_tiempo, char* pid){
    
    bool _es_interfaz_buscada(void* elemento){
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz* interfaz_encontrada =  malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);

    if(interfaz_encontrada != NULL){
        char* mensaje = string_new();
        string_append(&mensaje, "IO_GEN_SLEEP ");
        string_append(&mensaje, unidad_tiempo);
        string_append(&mensaje, " ");
        string_append(&mensaje, pid);
        sem_wait(&mutex_lista_interfaces);
        list_add(interfaz_encontrada->queue_instrucciones, mensaje);
        sem_post(&mutex_lista_interfaces);
        sem_post(&interfaz_encontrada->contador);
    }else{
        //Momentaneamente se usa sin finalizar proceso
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}


void liberar_interfaz(char * interfaz, char * pid, char* algoritmo){

    bool _es_interfaz_buscada(void* elemento){
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz* interfaz_encontrada =  malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);
    log_info(logger, "Kernel recibio la finalizacion de %s", interfaz);
    
    bool _es_pcb_buscado(void* elemento){
        return es_pcb_buscado(atoi(pid), elemento);
    }

    pcb_t* pcb_encontrado =  malloc(sizeof(pcb_t));
    sem_wait(&sem_array_estados[3].mutex);
    pcb_encontrado = list_remove_by_condition(QUEUE_BLOCKED, _es_pcb_buscado);
    sem_post(&sem_array_estados[3].mutex);
    sem_wait(&sem_array_estados[3].contador);
    
    if((strcmp(algoritmo, "VRR") == 0) && pcb_encontrado->quantum > 0){
        sem_wait(&sem_array_estados[5].mutex);
        list_add(QUEUE_READY_PLUS, pcb_encontrado);
        sem_post(&sem_array_estados[5].mutex);
        sem_post(&sem_array_estados[5].contador);

    }else{
        sem_wait(&sem_array_estados[1].mutex);
        list_add(QUEUE_READY, pcb_encontrado);
        sem_post(&sem_array_estados[1].mutex);
        sem_post(&sem_array_estados[1].contador);
    }

    sem_post(&interfaz_encontrada->sem_uso);
    
}

bool es_interfaz_buscada_socket (int socket, void* elemento){
    t_interfaz* aux = malloc(sizeof(t_interfaz));
    aux = elemento;
    bool aux2 = (aux->socket == socket);
    return (aux2);
}

void desconectar_interfaz(int socket_cliente){

    bool _es_interfaz_buscada_socket(void* elemento){
        return es_interfaz_buscada_socket(socket_cliente, elemento);
    }

    t_interfaz* interfaz_encontrada =  malloc(sizeof(t_interfaz));
    sem_wait(&mutex_lista_interfaces);
    interfaz_encontrada = list_remove_by_condition(INTERFACES, _es_interfaz_buscada_socket);
    sem_post(&mutex_lista_interfaces);
    if(interfaz_encontrada != NULL){
        log_info(logger, "Kernel recibio la desconexion de %d", socket_cliente);
        pthread_kill(interfaz_encontrada->id, 0);
    }
}