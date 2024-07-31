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

void wait_instruccion(char *nombre_recurso, int pid)
{
    if (existe_recurso(nombre_recurso))
    {

        bool _es_recurso_buscado(void *elemento)
        {
            return es_recurso_buscado(nombre_recurso, elemento);
        }

        recurso_t *recurso = list_find(lista_recursos, _es_recurso_buscado);

        sem_wait(&recurso->mutex_recurso);
        list_add(recurso->procesos_asignados_al_recurso, pid);
        recurso->instancias--;
        if (recurso->instancias < 0)
        {
            list_add(recurso->procesos_bloqueados, pid);
            bloquear_proceso(pid);
        }
        else
        {
            sem_wait(&sem_array_estados[2].contador);
            sem_wait(&sem_array_estados[2].mutex);
            sem_wait(&sem_array_estados[1].mutex);
            pcb_t *aux = list_remove(QUEUE_RUNNING, 0);
            list_add_in_index(QUEUE_READY, 0, aux);
            sem_post(&sem_array_estados[1].mutex);
            sem_post(&sem_array_estados[2].mutex);
            sem_post(&sem_array_estados[1].contador);
        }

        sem_post(&recurso->mutex_recurso);
    }
    else
    {
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

void signal_instruccion(char *nombre_recurso, int pid)
{

    bool _es_entero_buscado_recurso(void *elemento)
    {
        return es_entero_buscado_recurso(pid, elemento);
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
            list_add_in_index(QUEUE_READY_PLUS, 0, aux);
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
            list_add_in_index(QUEUE_READY, 0, aux);
            sem_post(&sem_array_estados[1].contador);
            sem_post(&sem_array_estados[1].mutex);
            sem_post(&sem_array_estados[2].mutex);
        }

        sem_post(&recurso->mutex_recurso);
    }
    else
    {
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
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
    sem_wait(&sem_array_estados[3].contador);
    pcb_t *aux = list_remove_by_condition(QUEUE_BLOCKED, _es_pid_buscado);
    list_add(QUEUE_READY, aux);
    sem_post(&sem_array_estados[1].contador);
    sem_post(&sem_array_estados[1].mutex);
    sem_post(&sem_array_estados[3].mutex);
}

void bloquear_proceso(int pid)
{
    bool _es_pid_buscado(void *elemento)
    {
        return es_pid_buscado_recurso(pid, elemento);
    }

    sem_wait(&sem_array_estados[3].mutex);
    sem_wait(&sem_array_estados[2].mutex);
    sem_wait(&sem_array_estados[2].contador);
    pcb_t *aux = list_remove_by_condition(QUEUE_RUNNING, _es_pid_buscado);
    list_add(QUEUE_BLOCKED, aux);
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