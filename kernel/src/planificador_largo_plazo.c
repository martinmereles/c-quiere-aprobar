#include "../include/planificador_largo_plazo.h"


void INICIAR_PROCESO (char* ruta) {

pcb_t* pcb_proceso = crear_pcb ()



} 

pcb_t* crear_pcb (){
    pcb_t* new_pcb = malloc(sizeof(pcb_t));
    new_pcb->registros_generales = malloc (sizeof(registros_t));
    GLOBAL_PID ++;
    new_pcb->pid = GLOBAL_PID;


}