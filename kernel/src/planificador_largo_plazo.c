#include "../include/planificador_largo_plazo.h"

void planificador_largo_plazo()
{
    while (1)
    {
        pasar_new_ready();
    }
}

void iniciar_proceso(char *ruta, char *quantum, int socket_cliente_memoria)
{   
    char* path = string_new();
    string_append(&path, ".");
    string_append(&path, ruta);
    int valor_quantum = atoi(quantum);
    pcb_t *pcb_proceso = crear_pcb(valor_quantum);
    sem_wait(&sem_array_estados[0].mutex);
    list_add(QUEUE_NEW, pcb_proceso);
    char *mensaje = string_new();
    string_append(&mensaje, "INICIAR_PROCESO ");
    string_append(&mensaje, string_itoa(pcb_proceso->pid));
    string_append(&mensaje, " ");
    string_append(&mensaje, path);
    enviar_mensaje(mensaje, socket_cliente_memoria);
    sem_post(&sem_array_estados[0].mutex);

    int cod_op = recibir_operacion(socket_cliente_memoria);
    switch (cod_op)
    {
    case MENSAJE:
        int size;
        char *buffer = recibir_buffer(&size, socket_cliente_memoria);
        char **mensaje_split = string_split(buffer, " ");
        if (strcmp(mensaje_split[0], "INICIAR_PROCESO") == 0)
        {
            sem_post(&sem_array_estados[0].contador);
            log_info(logger, "Se crea el proceso %d en NEW", pcb_proceso->pid);
        }
        break;
    case -1:
        log_error(logger, "El cliente se desconecto.");
        desconectar_interfaz(socket_cliente_memoria);
        return EXIT_FAILURE;
    default:
        log_warning(logger, "Operacion desconocida. No quieras meter la pata");
        break;
    }

    
}

pcb_t *crear_pcb(int quantum)
{
    pcb_t *new_pcb = malloc(sizeof(pcb_t));
    new_pcb->reg_generales = malloc(sizeof(registros_t));
    GLOBAL_PID++;
    new_pcb->PCB_PC = 0;
    new_pcb->pid = GLOBAL_PID;
    new_pcb->quantum = quantum;
    new_pcb->reg_generales->AX = (uint8_t)0;
    new_pcb->reg_generales->BX = (uint8_t)0;
    new_pcb->reg_generales->CX = (uint8_t)0;
    new_pcb->reg_generales->DX = (uint8_t)0;
    new_pcb->reg_generales->EAX = 0;
    new_pcb->reg_generales->EBX = 0;
    new_pcb->reg_generales->ECX = 0;
    new_pcb->reg_generales->EDX = 0;
    new_pcb->reg_generales->PC = 0;
    new_pcb->reg_generales->SI = 0;
    new_pcb->reg_generales->DI = 0;
    return new_pcb;
}

void pasar_new_ready()
{
    sem_wait(&sem_grado_multiprog);
    sem_wait(&detencion_planificador_largo);
    sem_post(&detencion_planificador_largo);
    pcb_t *aux = malloc(sizeof(pcb_t));
    sem_wait(&sem_array_estados[0].contador);
    sem_wait(&sem_array_estados[0].mutex);
    sem_wait(&sem_array_estados[1].mutex);
    aux = list_remove(QUEUE_NEW, 0);
    list_add(QUEUE_READY, aux);
    log_info(logger, "PID: %d - Estado Anterior: NEW - Estado Actual: READY", aux->pid);
    char* pids = generar_lista_pids("QUEUE_READY");
    log_info(logger, "Cola Ready: %s", pids);
    sem_post(&sem_array_estados[1].mutex);
    sem_post(&sem_array_estados[0].mutex);
    sem_post(&sem_array_estados[1].contador);
}