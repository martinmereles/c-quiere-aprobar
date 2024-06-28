
#include <../include/ciclo_instruccion.h>

void fetch(int socket_cliente_memoria){
    char* pid = string_itoa(contexto->pid);
    char* program_counter = string_itoa(reg->PC);
    char* mensaje = string_new();
    string_append(&mensaje,"PROXIMA_INSTRUCCION ");
    string_append(&mensaje, pid);
    string_append(&mensaje, " ");
    string_append(&mensaje, program_counter);
    enviar_mensaje(mensaje ,socket_cliente_memoria);
	atender_cliente_cpu(socket_cliente_memoria);
    reg->PC++;
}

void decode(int socket_cliente_memoria){
    //TO DO
}

void execute(int socket_cliente_kernel){
    ejecutarSentencia(socket_cliente_kernel);
    //Actualizamos contexto
    contexto->reg_generales = reg;

}

void check_interrupt(int socket_cliente_memoria, int socket_kernel_dispatch){
    
    bool existe_finalizar = false;
    bool existe_fin_quantum = false;
    bool existe_io = false;

    for(int i = 0; i < list_size(INTERRUPCIONES); i++){
        
        char ** interrupcion_split = string_split(list_get(INTERRUPCIONES, i), " ");

        if(!strcmp(interrupcion_split[0],"EXIT") && !strcmp(interrupcion_split[1], string_itoa(contexto->pid))){
            existe_finalizar = true;

        }else if(!strcmp(interrupcion_split[0],"FIN_QUANTUM") && !strcmp(interrupcion_split[1], string_itoa(contexto->pid))){
            existe_fin_quantum = true;

        }
        list_remove(INTERRUPCIONES, i);
    }
    
    char ** instruccion_exec_split = string_split(instruccion_exec, " ");
    if(!strcmp(instruccion_exec_split[0],"IO_GEN_SLEEP")){
        existe_io = true;
    }

    if(existe_finalizar){
        enviar_pcb_contexto_motivo(socket_kernel_dispatch, contexto, "INTERRUPTED_BY_USER");
        log_info (logger, "Finaliza el proceso %s - Motivo: INTERRUPTED_BY_USER", string_itoa(contexto->pid));
        sem_wait(&sem_execute);
    }else if(existe_io){
        enviar_pcb_contexto_motivo(socket_kernel_dispatch, contexto, "IO");
        log_info (logger, "PID: %s - Desalojado por IO", string_itoa(contexto->pid));
        sem_wait(&sem_execute);
    }
    else if(existe_fin_quantum){
        enviar_pcb_contexto_motivo(socket_kernel_dispatch, contexto, "FIN_QUANTUM");
        log_info (logger, "PID: %s - Desalojado por fin de Quantum", string_itoa(contexto->pid));
        sem_wait(&sem_execute);
    }
    
}