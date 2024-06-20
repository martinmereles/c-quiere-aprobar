#include "../include/consola_kernel.h"

void lanzar_consola (char* quantum, int socket_cliente_memoria, int socket_cpu_dispatch, int socket_cpu_interrupt, t_config* config) {
	char* command;
    command = readline("> ");
    while(1){
    	log_info(logger, "Se recibió el comando: %s", command);
        entender_comando(command, quantum, socket_cliente_memoria, socket_cpu_interrupt, config);
    	free(command);
    	command = readline("> ");
        //PARA PRUEBA ANTES DE ENVIAR DEJAMOS "CORRIENDO" EL PROCESO
        if(!strcmp(command,"INICIAR")){
        list_add(QUEUE_RUNNING, list_get(QUEUE_READY,0));
        list_remove(QUEUE_READY, 0);
        pcb_t* new_pcb = malloc(sizeof(pcb_t));
        new_pcb->reg_generales = malloc(sizeof(registros_t));
        new_pcb = list_get(QUEUE_RUNNING,0);
        new_pcb->reg_generales->EAX = 2;
        enviar_pcb_contexto(socket_cpu_dispatch, new_pcb);
        //enviar_pcb_contexto(socket_cpu_dispatch, list_get(QUEUE_RUNNING,0));
        }
    }
    free(command);
}

void entender_comando(char* command, char* quantum, int socket_cliente_memoria, int socket_cpu_interrupt, t_config* config){
    char ** command_split = string_split(command, " ");
    if(!strcmp(command_split[0],"EJECUTAR_SCRIPT")){
        ejecutar_script(command_split[1],quantum, socket_cliente_memoria, socket_cpu_interrupt,config);  
    };
    if(!strcmp(command_split[0],"INICIAR_PROCESO")){ 
        iniciar_proceso(command_split[1], quantum, socket_cliente_memoria);
    };
    if(!strcmp(command_split[0],"FINALIZAR_PROCESO")){
        finalizar_proceso(atoi(command_split[1]), socket_cpu_interrupt, socket_cliente_memoria);
    };
    if(!strcmp(command_split[0],"DETENER_PLANIFICACION")){
        
    };
    if(!strcmp(command_split[0],"INICIAR_PLANIFICACION")){
        iniciar_planificacion();
    };
    if(!strcmp(command_split[0],"MULTIPROGRAMACION")){
        
    };
    if(!strcmp(command_split[0],"PROCESO_ESTADO")){
        
    };
    if(!strcmp(command_split[0],"EXIT")){
    log_info(logger, "Se ejecutó la instrucción => EXIT");
    cerrar_log_config (logger,config); 
    list_destroy(QUEUE_NEW);
    list_destroy(QUEUE_READY);
    list_destroy(QUEUE_RUNNING);
    list_destroy(QUEUE_BLOCKED);
    list_destroy(QUEUE_TERMINATED);
    exit(-1);
    };
    
}

void iniciar_planificacion(){
   
}

void ejecutar_script(char* path, char* quantum, int socket_cliente_memoria, int socket_cpu_interrupt, t_config* config){
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }
    char linea[256];
    while(fgets(linea, sizeof(linea), f) != NULL){
        size_t tamano = strcspn(linea, "\n");
        char* frase = malloc(tamano);
        frase = string_substring(linea, 0, tamano);
        entender_comando(frase, quantum, socket_cliente_memoria, socket_cpu_interrupt, config);
        log_info(logger, "Se ejecutó la instrucción => %s",frase);
        free(frase);
    }
    fclose(f);
}

bool es_pid_buscado(int pid, void* elemento){
    pcb_t* aux = malloc(sizeof(pcb_t));
    aux = elemento;
    bool aux2 = aux->pid == pid;
    return (aux2);
}

void finalizar_proceso(int pid, int socket_cpu_interrupt, int socket_cliente_memoria){
    pcb_t* encontrado = malloc(sizeof(pcb_t));
    encontrado->reg_generales = malloc (sizeof(registros_t));
    encontrado = NULL;

    bool _es_pid_buscado(void* elemento){
        return es_pid_buscado(pid, elemento);
    }
    sem_wait(&sem_array_estados[0].mutex);
    sem_wait(&sem_array_estados[1].mutex);
    sem_wait(&sem_array_estados[2].mutex);
    sem_wait(&sem_array_estados[3].mutex);
    sem_wait(&sem_array_estados[4].mutex);
    encontrado = list_remove_by_condition(QUEUE_NEW, _es_pid_buscado);
    if(encontrado != NULL){
        sem_wait(&sem_array_estados[0].contador);
    }
    encontrado = NULL;
    encontrado = list_remove_by_condition(QUEUE_READY, _es_pid_buscado);
    if(encontrado != NULL){
        sem_wait(&sem_array_estados[1].contador);
        sem_post(&sem_grado_multiprog);
    }
    encontrado = NULL;
    encontrado = list_remove_by_condition(QUEUE_RUNNING, _es_pid_buscado);
    if(encontrado != NULL){
        char* mensaje = string_new();
        string_append(&mensaje, "EXIT ");
        string_append(&mensaje, string_itoa(pid));
        enviar_mensaje(mensaje, socket_cpu_interrupt);
        sem_wait(&sem_sincro_cpu);
        enviar_mensaje(mensaje, socket_cliente_memoria);
        sem_wait(&sem_array_estados[2].contador);
        sem_post(&sem_grado_multiprog);
    }
    encontrado = NULL;
    encontrado = list_remove_by_condition(QUEUE_BLOCKED, _es_pid_buscado);
    if(encontrado != NULL){
        sem_wait(&sem_array_estados[3].contador);
        sem_post(&sem_grado_multiprog);
    }
    encontrado = NULL;
    encontrado = list_remove_by_condition(QUEUE_TERMINATED, _es_pid_buscado);
    if(encontrado != NULL){
        sem_wait(&sem_array_estados[4].contador);
    }
    sem_post(&sem_array_estados[0].mutex);
    sem_post(&sem_array_estados[1].mutex);
    sem_post(&sem_array_estados[2].mutex);
    sem_post(&sem_array_estados[3].mutex);
    sem_post(&sem_array_estados[4].mutex);

    //free(encontrado->reg_generales);
    //free(encontrado);
}


