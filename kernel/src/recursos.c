#include <../include/recursos.h>

void cargar_recursos(t_config *config)
{
    char **recursos = config_get_array_value(config, "RECURSOS");
    char **instancias_recursos = config_get_array_value(config, "INSTANCIAS_RECURSOS");
    sem_wait(&mutex_lista_recursos);
    for (int i = 0; recursos[i] != NULL; i++)
    {
        
        recurso_t *recurso = malloc(sizeof(recurso_t));
        recurso->nombre = string_new();
        string_append(&recurso->nombre, recursos[i]);
        recurso->procesos_bloqueados = list_create();
        recurso->procesos_asignados_al_recurso = list_create();
        recurso->instancias = atoi(instancias_recursos[i]);
        sem_init(&recurso->mutex_recurso, 0, 1);
        list_add(lista_recursos, recurso);
    }
    sem_post(&mutex_lista_recursos);
}

bool existe_recurso(char *nombre_recurso)
{
    bool existe = false;
    recurso_t *recurso;
    for (int i = 0; i < list_size(lista_recursos); i++)
    {
        recurso = list_get(lista_recursos, i);
        if (strcmp(recurso->nombre, nombre_recurso) == 0)
        {
            existe = true;
        }
    }
    return existe;
}

void wait_instruccion(char *nombre_recurso, pcb_t* pcb_recibido)
{
    if (existe_recurso(nombre_recurso))
    {

        bool _es_recurso_buscado(void *elemento)
        {
            return es_recurso_buscado(nombre_recurso, elemento);
        }

        recurso_t *recurso = list_find(lista_recursos, _es_recurso_buscado);

        sem_wait(&recurso->mutex_recurso);
        list_add(recurso->procesos_asignados_al_recurso, pcb_recibido->pid);
        recurso->instancias--;
        if (recurso->instancias < 0)
        {
            list_add(recurso->procesos_bloqueados, pcb_recibido->pid);
            bloquear_proceso(pcb_recibido);
            log_info(logger, "PID: %s - Bloqueado por: %s", pcb_recibido->pid, nombre_recurso);
        }
        else
        {
            sem_wait(&sem_array_estados[2].contador);
            sem_wait(&sem_array_estados[2].mutex);
            pcb_t *aux = list_remove(QUEUE_RUNNING, 0);
            if (strcmp(algoritmo,"VRR")==0){
                sem_wait(&sem_array_estados[5].mutex);
                list_add_in_index(QUEUE_READY_PLUS, 0, aux);
                log_info(logger, "PID: %d - Estado Anterior: RUNNING - Estado Actual: READY_PLUS", aux->pid);
                char* pids = string_new();
                generar_lista_pids(&pids, "QUEUE_READY_PLUS");
                log_info(logger, "Cola Ready Prioridad: %s", pids);
                sem_post(&sem_array_estados[5].contador);
                sem_post(&sem_array_estados[5].mutex);
            }else{
                sem_wait(&sem_array_estados[1].mutex);
                list_add_in_index(QUEUE_READY, 0, aux);
                log_info(logger, "PID: %d - Estado Anterior: RUNNING - Estado Actual: READY", aux->pid);
                char* pids = string_new();
                generar_lista_pids(&pids, "QUEUE_READY");
                log_info(logger, "Cola Ready: %s", pids);
                sem_post(&sem_array_estados[1].contador);
                sem_post(&sem_array_estados[1].mutex);
            }
            sem_post(&sem_array_estados[2].mutex);
        }

        sem_post(&recurso->mutex_recurso);
    }
    else
    {
        finalizar_proceso_invalido(pcb_recibido->pid, socket_cpu_interrupt, socket_memoria, "INVALID_RESOURCE");
    }
}

void signal_instruccion(char *nombre_recurso, pcb_t* pcb_recibido)
{

    bool _es_entero_buscado_recurso(void *elemento)
    {
        return es_entero_buscado_recurso(pcb_recibido->pid, elemento);
    }

    if (existe_recurso(nombre_recurso))
    {

        bool _es_recurso_buscado(void *elemento)
        {
            return es_recurso_buscado(nombre_recurso, elemento);
        }

        recurso_t *recurso = list_find(lista_recursos, _es_recurso_buscado);

        sem_wait(&recurso->mutex_recurso);
        list_remove_by_condition(recurso->procesos_asignados_al_recurso, _es_entero_buscado_recurso);
        recurso->instancias++;
        if (list_size(recurso->procesos_bloqueados) > 0)
        {
            int pid_a_desbloquear = list_remove(recurso->procesos_bloqueados, 0);
            desbloquear_proceso(pid_a_desbloquear);
        }
        pcb_t *aux;
        if (strcmp(algoritmo,"VRR")==0)
        {

            sem_wait(&sem_array_estados[2].mutex);
            sem_wait(&sem_array_estados[5].mutex);
            sem_wait(&sem_array_estados[2].contador);
            aux = list_remove(QUEUE_RUNNING, 0);
            list_add_in_index(QUEUE_READY_PLUS, 0, pcb_recibido);
            log_info(logger, "PID: %d - Estado Anterior: RUNNING - Estado Actual: READY_PLUS", pcb_recibido->pid);
            char* pids = string_new();
            generar_lista_pids(&pids, "QUEUE_READY_PLUS");
            log_info(logger, "Cola Ready Prioridad: %s", pids);
            sem_post(&sem_array_estados[5].contador);
            sem_post(&sem_array_estados[5].mutex);
            sem_post(&sem_array_estados[2].mutex);
        }
        else
        {
            sem_wait(&sem_array_estados[2].mutex);
            sem_wait(&sem_array_estados[1].mutex);
            sem_wait(&sem_array_estados[2].contador);
            aux = list_remove(QUEUE_RUNNING, 0);
            list_add_in_index(QUEUE_READY, 0, pcb_recibido);
            log_info(logger, "PID: %d - Estado Anterior: RUNNING - Estado Actual: READY", pcb_recibido->pid);
            char* pids = string_new();
            generar_lista_pids(&pids, "QUEUE_READY");
            log_info(logger, "Cola Ready: %s", pids);
            sem_post(&sem_array_estados[1].contador);
            sem_post(&sem_array_estados[1].mutex);
            sem_post(&sem_array_estados[2].mutex);
        }

        sem_post(&recurso->mutex_recurso);
    }
    else
    {
        finalizar_proceso_invalido(pcb_recibido->pid, socket_cpu_interrupt, socket_memoria, "INVALID_RESOURCE");
    }
}

bool es_recurso_buscado(char *identificador, void *elemento)
{
    recurso_t *aux = malloc(sizeof(recurso_t));
    aux = elemento;
    bool aux2 = (strcmp(aux->nombre, identificador) == 0);
    return (aux2);
}

bool es_pid_buscado_recurso(int identificador, void *elemento)
{
    pcb_t *aux = malloc(sizeof(pcb_t));
    aux = elemento;
    bool aux2 = aux->pid == identificador;
    return (aux2);
}

void desbloquear_proceso(int pid)
{
    //  pcb_t* aux = malloc(sizeof(pcb_t));
    bool _es_pid_buscado(void *elemento)
    {
        return es_pid_buscado_recurso(pid, elemento);
    }

    
    sem_wait(&sem_array_estados[3].mutex);
    sem_wait(&sem_array_estados[1].mutex);
    sem_wait(&sem_array_estados[5].mutex);
    sem_wait(&sem_array_estados[3].contador);
    pcb_t *aux = list_remove_by_condition(QUEUE_BLOCKED, _es_pid_buscado);
    if (strcmp(algoritmo,"VRR")==0 && aux->quantum > 0){
        list_add(QUEUE_READY_PLUS, aux);
        log_info(logger, "PID: %d - Estado Anterior: BLOCKED - Estado Actual: READY_PLUS", aux->pid);
        char* pids = string_new();
        generar_lista_pids(&pids, "QUEUE_READY_PLUS");
        log_info(logger, "Cola Ready Prioridad: %s", pids);
        sem_post(&sem_array_estados[5].contador);
    }else{
        list_add(QUEUE_READY, aux);
        log_info(logger, "PID: %d - Estado Anterior: BLOCKED - Estado Actual: READY", aux->pid);
        char* pids = string_new();
        generar_lista_pids(&pids, "QUEUE_READY");
        log_info(logger, "Cola Ready: %s", pids);
        sem_post(&sem_array_estados[1].contador);
    }
    sem_post(&sem_array_estados[5].mutex);
    sem_post(&sem_array_estados[1].mutex);
    sem_post(&sem_array_estados[3].mutex);
}

void bloquear_proceso(pcb_t* pcb_recibido)
{
    bool _es_pid_buscado(void *elemento)
    {
        return es_pid_buscado_recurso(pcb_recibido->pid, elemento);
    }

    sem_wait(&sem_array_estados[3].mutex);
    sem_wait(&sem_array_estados[2].mutex);
    sem_wait(&sem_array_estados[2].contador);
    pcb_t *aux = list_remove_by_condition(QUEUE_RUNNING, _es_pid_buscado);
    list_add(QUEUE_BLOCKED, pcb_recibido);
    log_info(logger, "PID: %d - Estado Anterior: RUNNING - Estado Actual: BLOQUED", pcb_recibido->pid);
    sem_post(&sem_array_estados[3].contador);
    sem_post(&sem_array_estados[2].mutex);
    sem_post(&sem_array_estados[3].mutex);
}

bool es_entero_buscado_recurso(int identificador, void *elemento)
{
    int aux = elemento;
    bool aux2 = aux == identificador;
    return (aux2);
}