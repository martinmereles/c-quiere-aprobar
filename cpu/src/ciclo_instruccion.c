
#include "../include/ciclo_instruccion.h"

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

void execute(){
    //TO DO
    ejecutarSentencia(instruccion_exec);
}

void check_interrupt(int socket_cliente_memoria){
    //TO DO
}