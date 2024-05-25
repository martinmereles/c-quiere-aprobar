#include "../include/consola_kernel.h"

void lanzar_consola (char* quantum, int socket_cliente_memoria, t_config* config) {
	char* command;
    command = readline("> ");
    while(1){
    	log_info(logger, "Se recibió el comando: %s", command);
        entender_comando(command, quantum, socket_cliente_memoria, config);
    	free(command);
    	command = readline("> ");
    }
    free(command);
}

void entender_comando(char* command, char* quantum, int socket_cliente_memoria, t_config* config){
    char ** command_split = string_split(command, " ");
    if(!strcmp(command_split[0],"EJECUTAR_SCRIPT")){

    };
    if(!strcmp(command_split[0],"INICIAR_PROCESO")){ 
        iniciar_proceso(command_split[1], quantum, socket_cliente_memoria);
    };
    if(!strcmp(command_split[0],"FINALIZAR_PROCESO")){
        
    };
    if(!strcmp(command_split[0],"DETENER_PLANIFICACION")){
        
    };
    if(!strcmp(command_split[0],"INICIAR_PLANIFICACION")){
        
    };
    if(!strcmp(command_split[0],"MULTIPROGRAMACION")){
        
    };
    if(!strcmp(command_split[0],"PROCESO_ESTADO")){
        
    };
    if(!strcmp(command_split[0],"EXIT")){
    cerrar_log_config (logger,config); 
    list_destroy(QUEUE_NEW);
    list_destroy(QUEUE_READY);
    list_destroy(QUEUE_RUNNING);
    list_destroy(QUEUE_BLOCKED);
    list_destroy(QUEUE_TERMINATED);
    exit(-1);
    };
    
}