#include "../include/consola_kernel.h"

void lanzar_consola (char* quantum, int socket_cliente_memoria, int socket_cpu_dispatch, int socket_cpu_interrupt, t_config* config) {
	char* command;
    command = readline("> ");
    while(1){
    	log_info(logger, "Se recibió el comando: %s", command);
        entender_comando(command, quantum, socket_cliente_memoria, socket_cpu_interrupt, config);
    	free(command);
    	command = readline("> ");
        
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
        multiprogramacion(atoi(command_split[1]), config);
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
    sem_wait(&sem_array_estados[5].mutex);
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
    encontrado = NULL;
    encontrado = list_remove_by_condition(QUEUE_READY_PLUS, _es_pid_buscado);
    if(encontrado != NULL){
        sem_wait(&sem_array_estados[5].contador);
        sem_post(&sem_grado_multiprog);
    }
    sem_post(&sem_array_estados[0].mutex);
    sem_post(&sem_array_estados[1].mutex);
    sem_post(&sem_array_estados[2].mutex);
    sem_post(&sem_array_estados[3].mutex);
    sem_post(&sem_array_estados[4].mutex);
    sem_post(&sem_array_estados[5].mutex);

    //free(encontrado->reg_generales);
    //free(encontrado);
}


void iniciar_planificacion(){
   
}

void detener_planificacion(){

}

void multiprogramacion(int grado_multiprog_nuevo, t_config* config){
    int grado_multiprog_actual = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
    int grados_a_agregar = grado_multiprog_nuevo - grado_multiprog_actual;
    if(grados_a_agregar > 0){
        for(int i=0; i< grados_a_agregar; i++){
            sem_post(&sem_grado_multiprog);
        }

    }else if(grados_a_agregar < 0){
        for(int i=0; i> grados_a_agregar; i--){
            sem_wait(&sem_grado_multiprog);
        }
    }
}