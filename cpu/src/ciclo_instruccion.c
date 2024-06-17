
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
    /*
    for(int i=0; i< list_size(INTERRUPCIONES); i++){
        char ** interrupcion_split = string_split(list_get(INTERRUPCIONES, i), " ");
        if(!strcmp(interrupcion_split[0],"EXIT") && interrupcion_split[1] == string_itoa(contexto->pid)){
            contexto->motivo = MOTIVO_EXIT;
            enviar_pcb_contexto(contexto, socket_kernel_dispatch);
            sem_wait(&sem_execute);
        }
    }

    if(strcmp(instruccion_exec,"EXIT")==0){
        sem_wait(&sem_execute);
    }*/
}