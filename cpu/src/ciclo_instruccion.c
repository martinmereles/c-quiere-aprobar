
#include <../include/ciclo_instruccion.h>

void fetch(int socket_cliente_memoria){
    char* pid = string_itoa(contexto->pid);
    char* program_counter = string_itoa(reg->PC);
    log_info(logger, "PID: %s - FETCH - Program Counter: %s", pid, program_counter);
    char* mensaje = string_new();
    string_append(&mensaje,"PROXIMA_INSTRUCCION ");
    string_append(&mensaje, pid);
    string_append(&mensaje, " ");
    string_append(&mensaje, program_counter);
    enviar_mensaje(mensaje ,socket_cliente_memoria);
	atender_cliente_cpu(socket_cliente_memoria);
    reg->PC++;
}

void decode(int socket_cliente_memoria, char* algoritmo_tlb, int cantidad_entradas_tlb){
    
    list_clean(lista_marcos);
    list_clean(lista_marcos_destino);

    char ** instruccion_exec_split = string_split(instruccion_exec, " ");
    int direccion_fisica;
    char* instruccion_decode = string_new();
    if(strcmp(instruccion_exec_split[0],"MOV_IN") == 0){
        // MOV_IN EAX(registro_dato) 4(marco1-obligatorio) 8(marco2-opcional) 
        unsigned int valor_registro = get_valor_registro(instruccion_exec_split[2]);
        int tamanio_registro = get_tamanio_registro(instruccion_exec_split[1]);
        marcos_a_leer(contexto->pid, valor_registro, tamanio_registro, socket_cliente_memoria, algoritmo_tlb, cantidad_entradas_tlb);
    }
    if(strcmp(instruccion_exec_split[0],"MOV_OUT") == 0){
        //MOV_OUT
        unsigned int valor_registro = get_valor_registro(instruccion_exec_split[2]);
        int tamanio_registro = get_tamanio_registro(instruccion_exec_split[1]);
        marcos_a_leer(contexto->pid, valor_registro, tamanio_registro, socket_cliente_memoria, algoritmo_tlb, cantidad_entradas_tlb);
        
    }
    if(strcmp(instruccion_exec_split[0],"COPY_STRING") == 0){
        // COPY_STRING SI (registro_dato) 4(marco1-obligatorio) 8(marco2-opcional) 
        unsigned int valor_registro_si = get_valor_registro("SI");
        unsigned int valor_registro_di = get_valor_registro("DI");
        int tamanio_a_leer = atoi(instruccion_exec_split[1]);
        marcos_a_leer(contexto->pid, valor_registro_si, tamanio_a_leer, socket_cliente_memoria, algoritmo_tlb, cantidad_entradas_tlb);
        marcos_a_escribir(contexto->pid, valor_registro_di, tamanio_a_leer, socket_cliente_memoria, algoritmo_tlb, cantidad_entradas_tlb);
    }
     if(strcmp(instruccion_exec_split[0],"IO_STDIN_READ") == 0){
        unsigned int direccion = get_valor_registro(instruccion_exec_split[2]);
        unsigned int tamanio = get_valor_registro(instruccion_exec_split[3]);
        marcos_a_leer(contexto->pid, direccion, tamanio, socket_cliente_memoria, algoritmo_tlb, cantidad_entradas_tlb);
    }
    if(strcmp(instruccion_exec_split[0],"IO_STDOUT_WRITE") == 0){
        unsigned int direccion = get_valor_registro(instruccion_exec_split[2]);
        unsigned int tamanio = get_valor_registro(instruccion_exec_split[3]);
        marcos_a_leer(contexto->pid, direccion, tamanio, socket_cliente_memoria, algoritmo_tlb, cantidad_entradas_tlb);
    }
    if(strcmp(instruccion_exec_split[0],"IO_FS_WRITE") == 0){
        unsigned int direccion = get_valor_registro(instruccion_exec_split[3]);
        unsigned int tamanio = get_valor_registro(instruccion_exec_split[4]);
        marcos_a_leer(contexto->pid, direccion, tamanio, socket_cliente_memoria, algoritmo_tlb, cantidad_entradas_tlb);
    }
    if(strcmp(instruccion_exec_split[0],"IO_FS_READ") == 0){
        unsigned int direccion = get_valor_registro(instruccion_exec_split[3]);
        unsigned int tamanio = get_valor_registro(instruccion_exec_split[4]);
        marcos_a_leer(contexto->pid, direccion, tamanio, socket_cliente_memoria, algoritmo_tlb, cantidad_entradas_tlb);
    }
}

void execute(int socket_cliente_kernel, int socket_cliente_memoria){
    ejecutarSentencia(socket_cliente_kernel, socket_cliente_memoria);
    //Actualizamos contexto
    contexto->reg_generales = reg;

}

void check_interrupt(int socket_cliente_memoria, int socket_kernel_dispatch){
    
    bool existe_finalizar = false;
    bool existe_fin_quantum = false;
    bool existe_io = false;
    bool existe_recurso = false;

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
    if(!strcmp(instruccion_exec_split[0],"IO_GEN_SLEEP") ||
        !strcmp(instruccion_exec_split[0],"IO_STDIN_READ") ||
        !strcmp(instruccion_exec_split[0],"IO_STDOUT_WRITE") ||
        !strcmp(instruccion_exec_split[0],"IO_FS_CREATE") ||
        !strcmp(instruccion_exec_split[0],"IO_FS_DELETE") ||
        !strcmp(instruccion_exec_split[0],"IO_FS_TRUNCATE") ||
        !strcmp(instruccion_exec_split[0],"IO_FS_WRITE") ||
        !strcmp(instruccion_exec_split[0],"IO_FS_READ")){
        existe_io = true;
    }
    if(!strcmp(instruccion_exec_split[0],"WAIT") || !strcmp(instruccion_exec_split[0],"SIGNAL")){
        existe_recurso = true;
    }

    if(existe_finalizar){
        enviar_pcb_contexto_motivo(socket_kernel_dispatch, contexto, "INTERRUPTED_BY_USER");
        log_info (logger, "Finaliza el proceso %s - Motivo: INTERRUPTED_BY_USER", string_itoa(contexto->pid));
        sem_wait(&sem_execute);
    }else if(existe_io){
        enviar_pcb_contexto_motivo(socket_kernel_dispatch, contexto, "IO");
        log_info (logger, "PID: %s - Desalojado por IO", string_itoa(contexto->pid));
        sem_wait(&sem_execute);
    }else if (desalojo_out_of_memory){
        enviar_pcb_contexto_motivo(socket_kernel_dispatch, contexto, "OUT_OF_MEMORY");
        log_info (logger, "PID: %s - Desalojado por Out of Memory", string_itoa(contexto->pid));
        desalojo_out_of_memory = 0;
        sem_wait(&sem_execute);
    }
    else if(existe_fin_quantum){
        enviar_pcb_contexto_motivo(socket_kernel_dispatch, contexto, "FIN_QUANTUM");
        log_info (logger, "PID: %s - Desalojado por fin de Quantum", string_itoa(contexto->pid));
        sem_wait(&sem_execute);
    }else if(existe_recurso){

        
        char* nuevo_mensaje = string_new();
        string_append(&nuevo_mensaje, instruccion_exec_split[0]);
        string_append(&nuevo_mensaje, " ");
        string_append(&nuevo_mensaje, instruccion_exec_split[1]);
        enviar_pcb_contexto_motivo(socket_kernel_dispatch, contexto, nuevo_mensaje);
        log_info (logger, "PID: %s - Desalojado por syscall a recurso", string_itoa(contexto->pid));
        sem_wait(&sem_execute);
    }else if(strcmp(instruccion_exec_split[0],"EXIT") == 0){
        enviar_pcb_contexto_motivo(socket_kernel_dispatch, contexto, "SUCCESS");
        log_info (logger, "PID: %s - Desalojado por instruccion EXIT", string_itoa(contexto->pid));
        sem_wait(&sem_execute);
    }
    
    
}


unsigned int get_valor_registro(char * registro){
    if(strcmp(registro,"PC") == 0){return contexto->reg_generales->PC;}
    if(strcmp(registro,"AX") == 0){return contexto->reg_generales->AX;}
    if(strcmp(registro,"BX") == 0){return contexto->reg_generales->BX;}
    if(strcmp(registro,"CX") == 0){return contexto->reg_generales->CX;}
    if(strcmp(registro,"DX") == 0){return contexto->reg_generales->DX;}
    if(strcmp(registro,"EAX") == 0){return contexto->reg_generales->EAX;}
    if(strcmp(registro,"EBX") == 0){return contexto->reg_generales->EBX;}
    if(strcmp(registro,"ECX") == 0){return contexto->reg_generales->ECX;}
    if(strcmp(registro,"EDX") == 0){return contexto->reg_generales->EDX;}
    if(strcmp(registro,"SI") == 0){return contexto->reg_generales->SI;}
    if(strcmp(registro,"DI") == 0){return contexto->reg_generales->DI;}
}

void set_valor_registro(char * registro, void* dato){
    if(strcmp(registro,"PC") == 0){memcpy(contexto->reg_generales->PC, dato, 4);}
    if(strcmp(registro,"AX") == 0){memcpy(contexto->reg_generales->AX, dato, 1);}
    if(strcmp(registro,"BX") == 0){memcpy(contexto->reg_generales->BX, dato, 1);}
    if(strcmp(registro,"CX") == 0){memcpy(contexto->reg_generales->CX, dato, 1);}
    if(strcmp(registro,"DX") == 0){memcpy(contexto->reg_generales->DX, dato, 1);}
    if(strcmp(registro,"EAX") == 0){memcpy(contexto->reg_generales->EAX, dato, 4);}
    if(strcmp(registro,"EBX") == 0){memcpy(contexto->reg_generales->EBX, dato, 4);}
    if(strcmp(registro,"ECX") == 0){memcpy(contexto->reg_generales->ECX, dato, 4);}
    if(strcmp(registro,"EDX") == 0){memcpy(contexto->reg_generales->EDX, dato, 4);}
    if(strcmp(registro,"SI") == 0){memcpy(contexto->reg_generales->SI, dato, 4);}
    if(strcmp(registro,"DI") == 0){memcpy(contexto->reg_generales->DI, dato, 4);}
}

int get_tamanio_registro(char * registro){
    if(strcmp(registro,"AX") == 0 || strcmp(registro,"BX") == 0 || strcmp(registro,"CX") == 0 || strcmp(registro,"DX") == 0)
    {
        return 1;
    }
    else
    {
        return 4;
    }

}

