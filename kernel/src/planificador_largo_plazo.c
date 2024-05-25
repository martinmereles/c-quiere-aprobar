#include "../include/planificador_largo_plazo.h"

void iniciar_proceso (char* ruta, char* quantum, int socket_cliente_memoria) {
    int valor_quantum = atoi(quantum);
    pcb_t* pcb_proceso = crear_pcb (valor_quantum);
    list_add(QUEUE_NEW, pcb_proceso);
    char* mensaje = string_new();
    string_append(&mensaje,"INICIAR_PROCESO ");
    string_append(&mensaje, string_itoa(pcb_proceso->pid));
    string_append(&mensaje, " ");
    string_append(&mensaje, ruta);
    enviar_mensaje(mensaje ,socket_cliente_memoria);
} 

pcb_t* crear_pcb (int quantum){

    pcb_t* new_pcb = malloc(sizeof(pcb_t));
    new_pcb->reg_generales = malloc (sizeof(registros_t));
    GLOBAL_PID ++;
    new_pcb->pid = GLOBAL_PID;
    new_pcb->quantum = quantum;

    return new_pcb;
}