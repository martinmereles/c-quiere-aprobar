#include "../include/manage_clients.h"

void iniciar_hilo_server_kernel(char *puerto)
{
    int socket_servidor = iniciar_servidor(puerto);
    while (1)
    {
        pthread_t hiloAtencion;
        int *socket_cliente = (int)malloc(sizeof(int));
        socket_cliente = esperar_cliente(socket_servidor);
        pthread_create(&hiloAtencion,
                       NULL,
                       (void *)atender_cliente_kernel,
                       socket_cliente);
        pthread_detach(hiloAtencion);
    }
}

void atender_cliente_kernel(int socket_cliente)
{
    t_list *lista;
    while (1)
    {
        int cod_op = recibir_operacion(socket_cliente);
        switch (cod_op)
        {
        case MENSAJE:
            int size;
            char *buffer = recibir_buffer(&size, socket_cliente);
            log_info(logger, "Kernel recibio el mensaje %s", buffer);
            char **mensaje_split = string_split(buffer, " ");
            if (strcmp(mensaje_split[0], "IO_GEN_SLEEP") == 0)
            {
                io_gen_sleep(mensaje_split[1], mensaje_split[2], mensaje_split[3]);
            }
            if (strcmp(mensaje_split[0], "IO_STDIN_READ") == 0)
            {   
                io_stdin_read(mensaje_split[1], mensaje_split[2], buffer);
            }
            if (strcmp(mensaje_split[0], "IO_STDOUT_WRITE") == 0)
            {
                io_stdout_write(mensaje_split[1], mensaje_split[2], buffer);
            }
            if (strcmp(mensaje_split[0], "IO_FS_CREATE") == 0)
            {
                io_fs_create(mensaje_split[1], mensaje_split[2], mensaje_split[3]);
            }
            if (strcmp(mensaje_split[0], "IO_FS_DELETE") == 0)
            {
                io_fs_delete(mensaje_split[1], mensaje_split[2], mensaje_split[3]);
            }
            if (strcmp(mensaje_split[0], "IO_FS_TRUNCATE") == 0)
            {
                io_fs_truncate(mensaje_split[1], mensaje_split[2], mensaje_split[3], mensaje_split[4]);
            }
            if (strcmp(mensaje_split[0], "IO_FS_WRITE") == 0)
            {
                io_fs_write(mensaje_split[1], mensaje_split[2], mensaje_split[3], mensaje_split[4], buffer);
            }
            if (strcmp(mensaje_split[0], "IO_FS_READ") == 0)
            {
                io_fs_read(mensaje_split[1], mensaje_split[2], mensaje_split[3], mensaje_split[4], buffer);
            }
            if (strcmp(mensaje_split[0], "CONECTAR_INTERFAZ") == 0)
            {
                conectar_interfaz(mensaje_split[1], mensaje_split[2], socket_cliente);
            }
            if (strcmp(mensaje_split[0], "FIN_IO") == 0)
            {
                liberar_interfaz(mensaje_split[1], mensaje_split[2]);
            }

            


            free(buffer);
            break;
        case PAQUETE:
            int size2;

            pcb_motivo_t *pcb_deserealizado = malloc(sizeof(pcb_motivo_t));
            pcb_deserealizado->pcb = malloc(sizeof(pcb_t));
            pcb_deserealizado->pcb->reg_generales = malloc(sizeof(registros_t));
            pcb_deserealizado->motivo = string_new();

            pcb_deserealizado = recibir_buffer_pcb_motivo(&size2, socket_cliente);

            if (strcmp(pcb_deserealizado->motivo, "INTERRUPTED_BY_USER") == 0 ||
                strcmp(pcb_deserealizado->motivo, "OUT_OF_MEMORY") == 0 ||
                strcmp(pcb_deserealizado->motivo, "SUCCESS") == 0)
            {

                char *mensaje = string_new();
                string_append(&mensaje, "EXIT ");
                string_append(&mensaje, string_itoa(pcb_deserealizado->pcb->pid));
                enviar_mensaje(mensaje, socket_memoria);

                liberar_recursos(pcb_deserealizado->pcb->pid);

                

                sem_wait(&sem_array_estados[2].mutex);
                sem_wait(&sem_array_estados[4].mutex);
                sem_wait(&sem_array_estados[2].contador);
                list_remove(QUEUE_RUNNING, 0);
                list_add(QUEUE_TERMINATED, pcb_deserealizado->pcb);
                sem_post(&sem_array_estados[4].contador);
                sem_post(&sem_array_estados[4].mutex);
                sem_post(&sem_array_estados[2].mutex);

                sem_post(&sem_grado_multiprog);
                sem_post(&sem_multiprocesamiento);
                log_info(logger, "Finaliza el proceso %d - Motivo: %s", pcb_deserealizado->pcb->pid, pcb_deserealizado->motivo);
            }
            if (strcmp(pcb_deserealizado->motivo, "FIN_QUANTUM") == 0)
            {
                // agregar el pcb a READY

                sem_wait(&sem_array_estados[2].mutex);
                sem_wait(&sem_array_estados[1].mutex);
                sem_wait(&sem_array_estados[2].contador);
                list_remove(QUEUE_RUNNING, 0);
                list_add(QUEUE_READY, pcb_deserealizado->pcb);
                
                sem_post(&sem_array_estados[1].mutex);
                sem_post(&sem_array_estados[2].mutex);
                sem_post(&sem_array_estados[1].contador);

                sem_post(&sem_multiprocesamiento);
            }
            if (strcmp(pcb_deserealizado->motivo, "IO") == 0)
            {
                sem_wait(&sem_array_estados[3].mutex);
                sem_wait(&sem_array_estados[2].mutex);
                sem_wait(&sem_array_estados[2].contador);
                list_remove(QUEUE_RUNNING, 0);
                printf("PID DEL PCB DESEREALIZADO %d\n", pcb_deserealizado->pcb->pid);
                list_add(QUEUE_BLOCKED, pcb_deserealizado->pcb);
                
                sem_post(&sem_array_estados[2].mutex);
                sem_post(&sem_array_estados[3].mutex);
                sem_post(&sem_array_estados[3].contador);
                sem_post(&sem_multiprocesamiento);
            }
            if (string_starts_with(pcb_deserealizado->motivo, "SIGNAL"))
            {
                char **motivo_signal = string_split(pcb_deserealizado->motivo, " ");

                signal_instruccion(motivo_signal[1], pcb_deserealizado->pcb->pid);

                sem_post(&sem_multiprocesamiento);
            }
            if (string_starts_with(pcb_deserealizado->motivo, "WAIT"))
            {
                char **motivo_wait = string_split(pcb_deserealizado->motivo, " ");

                wait_instruccion(motivo_wait[1], pcb_deserealizado->pcb->pid);

                sem_post(&sem_multiprocesamiento);
            }

            // free (pcb);
            break;
        case -1:
            log_error(logger, "El cliente se desconecto.");
            desconectar_interfaz(socket_cliente);
            return EXIT_FAILURE;
        default:
            log_warning(logger, "Operacion desconocida. No quieras meter la pata");
            break;
        }
    }
}

void conectar_interfaz(char *tipo_interfaz, char *identificador, int socket_interfaz)
{
    t_interfaz *interfaz = malloc(sizeof(t_interfaz));

    interfaz->socket = socket_interfaz;
    interfaz->tipo_interfaz = tipo_interfaz;
    interfaz->identificador = identificador;
    interfaz->queue_instrucciones = list_create();
    sem_init(&interfaz->sem_uso, 0, 1);
    sem_init(&interfaz->contador, 0, 0);

    pthread_t hilo_Interfaz;
    pthread_create(&hilo_Interfaz,
                   NULL,
                   consumidor_interfaz,
                   interfaz);
    pthread_detach(hilo_Interfaz);

    interfaz->id = hilo_Interfaz;
    sem_wait(&mutex_lista_interfaces);
    list_add(INTERFACES, interfaz);
    sem_post(&mutex_lista_interfaces);
}

void consumidor_interfaz(t_interfaz *interfaz)
{
    while (1)
    {
        sem_wait(&interfaz->sem_uso);
        sem_wait(&interfaz->contador);
        int socket_interfaz = interfaz->socket;
        log_info(logger, "Se envio el mensaje a %s", interfaz->identificador);
        sem_wait(&mutex_lista_interfaces);
        char *mensaje = list_remove(interfaz->queue_instrucciones, 0);
        sem_post(&mutex_lista_interfaces);
        enviar_mensaje(mensaje, socket_interfaz);
    }
}

bool es_interfaz_buscada(char *identificador, void *elemento)
{
    t_interfaz *aux = malloc(sizeof(t_interfaz));
    aux = elemento;
    bool aux2 = (strcmp(aux->identificador, identificador) == 0);
    return (aux2);
}

bool es_pcb_buscado(int pid_buscado, void *elemento)
{
    pcb_t *aux = malloc(sizeof(pcb_t));
    aux = elemento;
    bool aux2 = (aux->pid == pid_buscado);
    return (aux2);
}

void io_gen_sleep(char *interfaz, char *unidad_tiempo, char *pid)
{

    bool _es_interfaz_buscada(void *elemento)
    {
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz *interfaz_encontrada = malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);

    if (interfaz_encontrada != NULL && admite_instruccion(interfaz_encontrada->tipo_interfaz, "IO_GEN_SLEEP"))
    {
        char *mensaje = string_new();
        string_append(&mensaje, "IO_GEN_SLEEP ");
        string_append(&mensaje, unidad_tiempo);
        string_append(&mensaje, " ");
        string_append(&mensaje, pid);
        sem_wait(&mutex_lista_interfaces);
        list_add(interfaz_encontrada->queue_instrucciones, mensaje);
        sem_post(&mutex_lista_interfaces);
        sem_post(&interfaz_encontrada->contador);
    }
    else
    {
        // Momentaneamente se usa sin finalizar proceso
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

void io_stdin_read(char *interfaz, char *pid, char* buffer)
{

    bool _es_interfaz_buscada(void *elemento)
    {
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz *interfaz_encontrada = malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);

    if (interfaz_encontrada != NULL && admite_instruccion(interfaz_encontrada->tipo_interfaz, "IO_STDIN_READ"))
    {
       
        char *mensaje = string_new();
        string_append(&mensaje, buffer);
        sem_wait(&mutex_lista_interfaces);
        list_add(interfaz_encontrada->queue_instrucciones, mensaje);
        sem_post(&mutex_lista_interfaces);
        sem_post(&interfaz_encontrada->contador);

        
    }
    else
    {
        // Momentaneamente se usa sin finalizar proceso
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

void io_stdout_write(char *interfaz, char *pid, char* buffer)
{

    bool _es_interfaz_buscada(void *elemento)
    {
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz *interfaz_encontrada = malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);

    if (interfaz_encontrada != NULL && admite_instruccion(interfaz_encontrada->tipo_interfaz, "IO_STDOUT_WRITE"))
    {
        char *mensaje = string_new();
        string_append(&mensaje, buffer);
        sem_wait(&mutex_lista_interfaces);
        list_add(interfaz_encontrada->queue_instrucciones, mensaje);
        sem_post(&mutex_lista_interfaces);
        sem_post(&interfaz_encontrada->contador);
    }
    else
    {
        // Momentaneamente se usa sin finalizar proceso
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

void io_fs_create(char *interfaz, char *nombre_archivo, char *pid)
{

    bool _es_interfaz_buscada(void *elemento)
    {
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz *interfaz_encontrada = malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);

    if (interfaz_encontrada != NULL && admite_instruccion(interfaz_encontrada->tipo_interfaz, "IO_FS_CREATE"))
    {
        char *mensaje = string_new();
        string_append(&mensaje, "IO_FS_CREATE ");
        string_append(&mensaje, nombre_archivo);
        string_append(&mensaje, " ");
        string_append(&mensaje, pid);
        sem_wait(&mutex_lista_interfaces);
        list_add(interfaz_encontrada->queue_instrucciones, mensaje);
        sem_post(&mutex_lista_interfaces);
        sem_post(&interfaz_encontrada->contador);
    }
    else
    {
        
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

void io_fs_delete(char *interfaz, char *nombre_archivo, char *pid)
{

    bool _es_interfaz_buscada(void *elemento)
    {
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz *interfaz_encontrada = malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);

    if (interfaz_encontrada != NULL && admite_instruccion(interfaz_encontrada->tipo_interfaz, "IO_FS_DELETE"))
    {
        char *mensaje = string_new();
        string_append(&mensaje, "IO_FS_DELETE ");
        string_append(&mensaje, nombre_archivo);
        string_append(&mensaje, " ");
        string_append(&mensaje, pid);
        sem_wait(&mutex_lista_interfaces);
        list_add(interfaz_encontrada->queue_instrucciones, mensaje);
        sem_post(&mutex_lista_interfaces);
        sem_post(&interfaz_encontrada->contador);
    }
    else
    {
        // Momentaneamente se usa sin finalizar proceso
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

void io_fs_truncate(char *interfaz, char *nombre_archivo, char *tamanio_a_truncar, char *pid)
{

    bool _es_interfaz_buscada(void *elemento)
    {
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz *interfaz_encontrada = malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);

    if (interfaz_encontrada != NULL && admite_instruccion(interfaz_encontrada->tipo_interfaz, "IO_FS_TRUNCATE"))
    {
        char *mensaje = string_new();
        string_append(&mensaje, "IO_FS_TRUNCATE ");
        string_append(&mensaje, nombre_archivo);
        string_append(&mensaje, " ");
        string_append(&mensaje, tamanio_a_truncar);
        string_append(&mensaje, " ");
        string_append(&mensaje, pid);
        sem_wait(&mutex_lista_interfaces);
        list_add(interfaz_encontrada->queue_instrucciones, mensaje);
        sem_post(&mutex_lista_interfaces);
        sem_post(&interfaz_encontrada->contador);
    }
    else
    {
        // Momentaneamente se usa sin finalizar proceso
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

void io_fs_write(char *interfaz, char *nombre_archivo, char *puntero_archivo, char *pid, char* buffer)
{

    bool _es_interfaz_buscada(void *elemento)
    {
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz *interfaz_encontrada = malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);

    if (interfaz_encontrada != NULL && admite_instruccion(interfaz_encontrada->tipo_interfaz, "IO_FS_WRITE"))
    {
        char *mensaje = string_new();
        string_append(&mensaje, buffer);
        sem_wait(&mutex_lista_interfaces);
        list_add(interfaz_encontrada->queue_instrucciones, mensaje);
        sem_post(&mutex_lista_interfaces);
        sem_post(&interfaz_encontrada->contador);
    }
    else
    {
        // Momentaneamente se usa sin finalizar proceso
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

void io_fs_read(char *interfaz, char *nombre_archivo, char *puntero_archivo, char *pid, char* buffer)
{

    bool _es_interfaz_buscada(void *elemento)
    {
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz *interfaz_encontrada = malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);

    if (interfaz_encontrada != NULL && admite_instruccion(interfaz_encontrada->tipo_interfaz, "IO_FS_READ"))
    {
        char *mensaje = string_new();
        string_append(&mensaje, buffer);
        sem_wait(&mutex_lista_interfaces);
        list_add(interfaz_encontrada->queue_instrucciones, mensaje);
        sem_post(&mutex_lista_interfaces);
        sem_post(&interfaz_encontrada->contador);
    }
    else
    {
        // Momentaneamente se usa sin finalizar proceso
        finalizar_proceso(pid, socket_cpu_interrupt, socket_memoria);
    }
}

void liberar_interfaz(char *interfaz, char *pid)
{

    bool _es_interfaz_buscada(void *elemento)
    {
        return es_interfaz_buscada(interfaz, elemento);
    }

    t_interfaz *interfaz_encontrada = malloc(sizeof(t_interfaz));
    interfaz_encontrada = list_find(INTERFACES, _es_interfaz_buscada);
    log_info(logger, "Kernel recibio la finalizacion de %s", interfaz);

    bool _es_pcb_buscado(void *elemento)
    {
        return es_pcb_buscado(atoi(pid), elemento);
    }

    //pcb_t *pcb_encontrado = malloc(sizeof(pcb_t));
    sem_wait(&sem_array_estados[3].mutex);
    pcb_t * pcb_encontrado = list_remove_by_condition(QUEUE_BLOCKED, _es_pcb_buscado);

    sem_post(&sem_array_estados[3].mutex);
    sem_wait(&sem_array_estados[3].contador);

    if ((strcmp(algoritmo, "VRR") == 0) && pcb_encontrado->quantum > 0)
    {
        sem_wait(&sem_array_estados[5].mutex);
        list_add(QUEUE_READY_PLUS, pcb_encontrado);
        sem_post(&sem_array_estados[5].mutex);
        sem_post(&sem_array_estados[5].contador);
    }
    else
    {
        sem_wait(&sem_array_estados[1].mutex);
        list_add(QUEUE_READY, pcb_encontrado);
        sem_post(&sem_array_estados[1].mutex);
        sem_post(&sem_array_estados[1].contador);
    }

    sem_post(&interfaz_encontrada->sem_uso);
}

bool es_interfaz_buscada_socket(int socket, void *elemento)
{
    t_interfaz *aux = malloc(sizeof(t_interfaz));
    aux = elemento;
    bool aux2 = (aux->socket == socket);
    return (aux2);
}

void desconectar_interfaz(int socket_cliente)
{

    bool _es_interfaz_buscada_socket(void *elemento)
    {
        return es_interfaz_buscada_socket(socket_cliente, elemento);
    }

    t_interfaz *interfaz_encontrada = malloc(sizeof(t_interfaz));
    sem_wait(&mutex_lista_interfaces);
    interfaz_encontrada = list_remove_by_condition(INTERFACES, _es_interfaz_buscada_socket);
    sem_post(&mutex_lista_interfaces);
    if (interfaz_encontrada != NULL)
    {
        log_info(logger, "Kernel recibio la desconexion de %d", socket_cliente);
        pthread_kill(interfaz_encontrada->id, 0);
    }
}

bool admite_instruccion(char *tipo, char *instruccion)
{
    if (strcmp(tipo, "GENERICA") == 0 && strcmp(instruccion, "IO_GEN_SLEEP") == 0)
    {
        return true;
    }
    else if (strcmp(tipo, "STDIN") == 0 && strcmp(instruccion, "IO_STDIN_READ") == 0)
    {
        return true;
    }
    else if (strcmp(tipo, "STDOUT") == 0 && strcmp(instruccion, "IO_STDOUT_WRITE") == 0)
    {
        return true;
    }
    else if (strcmp(tipo, "DIALFS") == 0 && (strcmp(instruccion, "IO_FS_CREATE") == 0 ||
                                             strcmp(instruccion, "IO_FS_DELETE") == 0 ||
                                             strcmp(instruccion, "IO_FS_TRUNCATE") == 0 ||
                                             strcmp(instruccion, "IO_FS_WRITE") == 0 ||
                                             strcmp(instruccion, "IO_FS_READ") == 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void liberar_recursos(int pid)
{

    bool _es_entero_buscado_recurso(void *elemento)
    {
        return es_entero_buscado_recurso(pid, elemento);
    }

    int cantidad_recursos = list_size(lista_recursos);

    for (int i = 0; i < cantidad_recursos; i++)
    {

        recurso_t *recurso = list_get(lista_recursos, i);
        for (int j = 0; j < list_size(recurso->procesos_asignados_al_recurso); j)
        {
            int process_id = list_get(recurso->procesos_asignados_al_recurso, j);
            if (process_id == pid)
            {

                sem_wait(&recurso->mutex_recurso);
                list_remove_by_condition(recurso->procesos_asignados_al_recurso, _es_entero_buscado_recurso);
                recurso->instancias++;
                if (list_size(recurso->procesos_bloqueados) > 0)
                {
                    int pid_a_desbloquear = list_remove(recurso->procesos_bloqueados, 0);
                    desbloquear_proceso(pid_a_desbloquear);
                }

                sem_post(&recurso->mutex_recurso);
            }
            else
            {
                j++;
            }
        }
    }
}