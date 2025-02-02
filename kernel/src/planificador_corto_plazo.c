#include "../include/planificador_corto_plazo.h"

void planificador_corto_plazo(socket_info_t* params){

    int socket_cpu_dispatch = params->socket_dispatch; 
    int socket_cpu_interrupt = params->socket_interrupt;

    t_config* config = iniciar_config(logger, "./kernel.config");
    char* quantum = config_get_string_value(config, "QUANTUM");
    int quantum_int = atoi(quantum);

    
    if(!strcmp(algoritmo,"FIFO")){
        ejecutar_fifo(socket_cpu_dispatch);
    }else if(!strcmp(algoritmo,"RR")){
        ejecutar_round_robin(socket_cpu_dispatch, socket_cpu_interrupt, quantum_int);
    }else if(!strcmp(algoritmo,"VRR")){
        ejecutar_virtual_rr(socket_cpu_dispatch, socket_cpu_interrupt, quantum_int);
    }
    config_destroy(config);

}

void ejecutar_fifo(int socket_cpu_dispatch){
    while(1){        
        sem_wait(&sem_multiprocesamiento);
        sem_wait(&detencion_planificador_corto);
        sem_post(&detencion_planificador_corto);
        sem_wait(&sem_array_estados[1].contador);
        // sacar el primero de READY y pasarlo a RUNNING
        sem_wait(&sem_array_estados[1].mutex);
        sem_wait(&sem_array_estados[2].mutex);
        pcb_t* pcb_a_enviar = malloc(sizeof(pcb_t));
        pcb_a_enviar->reg_generales = malloc(sizeof(registros_t));
        pcb_a_enviar = list_remove(QUEUE_READY, 0);
        list_add(QUEUE_RUNNING, pcb_a_enviar);
        log_info(logger, "PID: %d - Estado Anterior: READY - Estado Actual: RUNNING", pcb_a_enviar->pid);
        sem_post(&sem_array_estados[1].mutex);
        sem_post(&sem_array_estados[2].mutex);
        sem_post(&sem_array_estados[2].contador);
        dispatcher(pcb_a_enviar, socket_cpu_dispatch);
        
    }

}

void ejecutar_round_robin(int socket_cpu_dispatch, int socket_cpu_interrupt, int quantum_int){
    while(1){
        sem_wait(&sem_multiprocesamiento);
        sem_wait(&detencion_planificador_corto);
        sem_post(&detencion_planificador_corto);
        sem_wait(&sem_array_estados[1].contador);
        //sacar el primero de READY y pasarlo a RUNNING
        sem_wait(&sem_array_estados[1].mutex);
        sem_wait(&sem_array_estados[2].mutex);
        
        
        
        //pcb_t* pcb_a_enviar = malloc(sizeof(pcb_t));
        //pcb_a_enviar->reg_generales = malloc(sizeof(registros_t));
        
       
        
        pcb_t* pcb_a_enviar = list_remove(QUEUE_READY, 0);
        list_add(QUEUE_RUNNING, pcb_a_enviar);
        log_info(logger, "PID: %d - Estado Anterior: READY - Estado Actual: RUNNING", pcb_a_enviar->pid);
        
        sem_post(&sem_array_estados[1].mutex);
        sem_post(&sem_array_estados[2].mutex);
        sem_post(&sem_array_estados[2].contador);
        pcb_a_enviar->quantum = quantum_int;
        dispatcher(pcb_a_enviar, socket_cpu_dispatch);

        char* mensaje = string_new();
        string_append(&mensaje, "FIN_QUANTUM ");
        string_append(&mensaje, string_itoa(pcb_a_enviar->pid));
        //Inicio hilo quantum
       	pthread_t hilo_quantum;
	    pthread_create(&hilo_quantum,
                        NULL,
                        aviso_quantum,
                        mensaje);
	    pthread_detach(hilo_quantum);
        

    }
}

void ejecutar_virtual_rr(int socket_cpu_dispatch, int socket_cpu_interrupt, int quantum_int){
    
    while(1){
        sem_wait(&sem_multiprocesamiento);
        sem_wait(&detencion_planificador_corto);
        sem_post(&detencion_planificador_corto);
        pcb_t* pcb_a_enviar = malloc(sizeof(pcb_t));
        pcb_a_enviar->reg_generales = malloc(sizeof(registros_t));
        //sacar el primero de READY o READY+(si existe algun elemento) y pasarlo a RUNNING
        while(1){
        if(list_size(QUEUE_READY_PLUS)>0){
            sem_wait(&sem_array_estados[5].contador);
            sem_wait(&sem_array_estados[5].mutex);
            pcb_a_enviar = list_remove(QUEUE_READY_PLUS, 0);
            log_info(logger, "PID: %d - Estado Anterior: READY_PLUS - Estado Actual: RUNNING", pcb_a_enviar->pid);
            sem_post(&sem_array_estados[5].mutex);
            break;
        }else if(list_size(QUEUE_READY)>0){
            sem_wait(&sem_array_estados[1].contador);
            sem_wait(&sem_array_estados[1].mutex);
            pcb_a_enviar = list_remove(QUEUE_READY, 0);
            log_info(logger, "PID: %d - Estado Anterior: READY - Estado Actual: RUNNING", pcb_a_enviar->pid);
            pcb_a_enviar->quantum = quantum_int;
            sem_post(&sem_array_estados[1].mutex);
            break;
        }
        }
        sem_wait(&sem_array_estados[2].mutex);
        list_add(QUEUE_RUNNING, pcb_a_enviar);
        sem_post(&sem_array_estados[2].mutex);
        sem_post(&sem_array_estados[2].contador);
        
        dispatcher(pcb_a_enviar, socket_cpu_dispatch);

        char* mensaje = string_new();
        string_append(&mensaje, "FIN_QUANTUM ");
        string_append(&mensaje, string_itoa(pcb_a_enviar->pid));
        //Inicio hilo quantum
       	pthread_t hilo_quantum;
	    pthread_create(&hilo_quantum,
                        NULL,
                        aviso_quantum,
                        mensaje);
	    pthread_detach(hilo_quantum);
        

    }
}

void dispatcher(pcb_t* pcb_a_enviar, int socket_cpu_dispatch){
    enviar_pcb_contexto(socket_cpu_dispatch, pcb_a_enviar);
}

void aviso_quantum(char* mensaje){
        t_config* config = iniciar_config(logger, "./kernel.config");
        char* quantum = config_get_string_value(config, "QUANTUM");
        int quantum_int = atoi(quantum);
        int quantum_milisecons = quantum_int * 1000;
        usleep(quantum_milisecons);
        enviar_mensaje(mensaje, socket_cpu_interrupt);
        log_info(logger, "Se envia el mensaje %s" , mensaje);
}