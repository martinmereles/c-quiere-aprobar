#include "../include/auxiliares.h" 

void atender_cliente_cpu(int socket_cliente){
	t_list* lista;
        int cod_op = recibir_operacion(socket_cliente);; 
        switch (cod_op) {
        case MENSAJE:
            int size;
            char* buffer = recibir_buffer(&size, socket_cliente);
            log_info(logger, "Me llego el mensaje %s", buffer);
            char ** mensaje_split = string_split(buffer, " ");
            if (es_intruccion(mensaje_split[0]))
            {
            realloc(instruccion_exec,&size);
            strcpy(instruccion_exec,buffer);
            }
            
            
            free(buffer);
            break;
        case PAQUETE:
            lista = recibir_paquete(socket_cliente );
            log_info(logger, "Me llegaron los siguientes valores:\n");
            list_iterate(lista, (void*) iterator);
            break;
        case -1:
            log_error(logger, "el cliente se desconecto.");
            return EXIT_FAILURE;
        default:
            log_warning(logger,"Operacion desconocida. No quieras meter la pata");
            break;
        }
    
}

bool es_intruccion (char* palabra) {

    if (
    strcmp(palabra,"SET")==0
    ||strcmp(palabra,"MOV_IN")==0
    ||strcmp(palabra,"MOV_OUT")==0
    ||strcmp(palabra,"SUM")==0
    ||strcmp(palabra,"SUB")==0
    ||strcmp(palabra,"JNZ")==0
    ||strcmp(palabra,"RESIZE")==0
    ||strcmp(palabra,"COPY_STRING")==0
    ||strcmp(palabra,"WAIT")==0
    ||strcmp(palabra,"SIGNAL")==0
    ||strcmp(palabra,"IO_GEN_SLEEP")==0
    ||strcmp(palabra,"IO_STDIN_READ")==0
    ||strcmp(palabra,"IO_STDOUT_WRITE")==0
    ||strcmp(palabra,"IO_FS_CREATE")==0
    ||strcmp(palabra,"IO_FS_DELETE")==0
    ||strcmp(palabra,"IO_FS_TRUNCATE")==0
    ||strcmp(palabra,"IO_FS_WRITE")==0
    ||strcmp(palabra,"IO_FS_READ")==0
    ||strcmp(palabra,"EXIT")==0)
    {
        return true;
    }
    else {
        return false;
    }
}

void devolver_contexto_a_kernel (int socket_kernel) {

    contexto->reg_generales = reg;

    enviar_pcb_contexto(socket_kernel);

}



void enviar_pcb_contexto(int socket_kernel){

    t_paquete* paquete_pcb = crear_paquete();
    agregar_a_paquete(paquete_pcb, contexto, sizeof(pcb_t));
    agregar_a_paquete(paquete_pcb, contexto->reg_generales, sizeof(registros_t));

    log_info (logger, "Se enviara el PCB con id: %d a Kernel", contexto->pid);
    
    enviar_paquete(paquete_pcb, socket_kernel);    

    eliminar_paquete (paquete_pcb);
}