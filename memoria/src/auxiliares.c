#include "../include/auxiliares.h"

void iniciar_hilo_server_memoria(char *puerto)
{
    int socket_servidor = iniciar_servidor(puerto);
    while (1)
    {
        pthread_t hiloAtencion;
        int *socket_cliente = (int)malloc(sizeof(int));
        socket_cliente = esperar_cliente(socket_servidor);
        pthread_create(&hiloAtencion,
                       NULL,
                       (void *)atender_cliente_memoria,
                       socket_cliente);
        pthread_detach(hiloAtencion);
    }
}

void atender_cliente_memoria(int socket_cliente)
{
    t_list *lista;
    while (1)
    {
        int cod_op = recibir_operacion(socket_cliente);
        ;
        switch (cod_op)
        {
        case MENSAJE:
            int size;
            char *buffer = recibir_buffer(&size, socket_cliente);
            log_info(logger, "Me llego el mensaje %s", buffer);
            char **mensaje_split = string_split(buffer, " ");
            if (strcmp(mensaje_split[0], "INICIAR_PROCESO") == 0)
            {
                iniciar_proceso(mensaje_split[1], mensaje_split[2]);
            }
            if (strcmp(mensaje_split[0], "PROXIMA_INSTRUCCION") == 0)
            {
                proxima_instruccion(mensaje_split[1], mensaje_split[2], socket_cliente);
            }
            if (strcmp(mensaje_split[0], "TAM_PAGINA") == 0)
            {
                enviar_tamanio_pagina(socket_cliente);
            }
            if (strcmp(mensaje_split[0], "EXIT") == 0)
            {
                finalizar_proceso(atoi(mensaje_split[1]));
            }
            if (strcmp(mensaje_split[0], "RESIZE") == 0)
            {
                resize(atoi(mensaje_split[1]), atoi(mensaje_split[2]), socket_cliente);
            }
            if (strcmp(mensaje_split[0], "OBTENER_MARCO") == 0)
            {
                obtener_marco(atoi(mensaje_split[1]), atoi(mensaje_split[2]), socket_cliente);
            }
            if (strcmp(mensaje_split[0], "LEER") == 0)
            {
                leer(atoi(mensaje_split[1]), atoi(mensaje_split[2]), atoi(mensaje_split[3]), socket_cliente);
            }
            if (strcmp(mensaje_split[0], "ESCRIBIR") == 0)
            {   mem_hexdump(buffer, size);
                void* valor = malloc(atoi(mensaje_split[2]));
                memcpy(valor, buffer + (string_length(buffer)-atoi(mensaje_split[2])), atoi(mensaje_split[2]));
                escribir(atoi(mensaje_split[1]), atoi(mensaje_split[2]), atoi(mensaje_split[3]), valor, socket_cliente);
                
            }
             if (strcmp(mensaje_split[0], "COPY_STRING") == 0)
            {
                leer(atoi(mensaje_split[1]), atoi(mensaje_split[2]), atoi(mensaje_split[3]), socket_cliente);
            }
            free(buffer);
            break;
        case PAQUETE:
            lista = recibir_paquete(socket_cliente);
            log_info(logger, "Me llegaron los siguientes valores:\n");
            list_iterate(lista, (void *)iterator);
            break;
        case -1:
            log_error(logger, "El cliente se desconecto.");
            return EXIT_FAILURE;
        default:
            log_warning(logger, "Operacion desconocida. No quieras meter la pata");
            break;
        }
    }
}

void iniciar_proceso(char *process_id, char *path)
{
    int process = atoi(process_id);
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    t_instruccion_memoria *instruccion = malloc(sizeof(t_instruccion_memoria));
    instruccion->lista_instrucciones = list_create();
    instruccion->lista_paginas = list_create();
    instruccion->memoria_reservada = 0;

    instruccion->process_id = process;

    fseek(f, 0L, SEEK_END);
    int size_file = ftell(f);
    rewind(f);
    char *texto_completo = malloc(size_file + 1);
    size_t bytes_leidos = fread(texto_completo, sizeof(char), size_file, f);
    texto_completo[bytes_leidos] = '\0';
    char **instrucciones_split = string_array_new();
    instrucciones_split = string_split(texto_completo, "\n");

    int i = 0;
    int length = string_array_size(instrucciones_split);
    while (i < length)
    {
        list_add(instruccion->lista_instrucciones, instrucciones_split[i]);
        log_info(logger, "Instrucción cargada=> %s - Proceso %d", instrucciones_split[i], process);
        i++;
    }

    list_add(memoria_instrucciones, instruccion);
    fclose(f);
}

void proxima_instruccion(char *process_id_find, char *program_counter, int socket_cliente)
{

    t_instruccion_memoria *proceso = malloc(sizeof(t_instruccion_memoria));
    proceso->lista_instrucciones = list_create();
    int i = 0;
    int pid = 0;
    bool encontrado = false;
    while (!encontrado && list_size(memoria_instrucciones) > i)
    {
        proceso = list_get(memoria_instrucciones, i);
        pid = proceso->process_id;
        if (pid == atoi(process_id_find))
        {
            encontrado = true;
        }
        else
        {
            i++;
        }
    }
    if (!encontrado)
    {
        log_error(logger, "No existe el PID %s", process_id_find);
    }
    else
    {
        int pc = atoi(program_counter);
        char *instruccion = list_get(proceso->lista_instrucciones, pc);
        log_info(logger, "Instruccion a devolver=> %s", instruccion);
        log_info(logger, "Retardo %d ", retardo_respuesta);
        usleep(retardo_respuesta * 1000);
        enviar_mensaje(instruccion, socket_cliente);
    }
}

void enviar_tamanio_pagina(int socket_cliente)
{

    char *mensaje = string_new();
    string_append(&mensaje, "TAM_PAGINA ");
    string_append(&mensaje, string_itoa(tamanio_pagina));
    enviar_mensaje(mensaje, socket_cliente);
}

void resize(int tamanio, int pid, int socket_cliente)
{

    if (tamanio > 0)
    {
        int cantidad_marcos_solicitados = floor(tamanio / tamanio_pagina);
        if (tamanio % tamanio_pagina > 0)
        {
            cantidad_marcos_solicitados++;
        }
        int marcos_libres = cantidad_marcos_libres();
        int marcos_asignados = cantidad_marcos_asignados(pid);
        int marcos_a_agregar = cantidad_marcos_solicitados - marcos_asignados;
        bool _es_pid_buscado(void *elemento)
        {
            return es_pid_buscado(pid, elemento);
        };
        t_instruccion_memoria *aux = list_find(memoria_instrucciones, _es_pid_buscado);

        if (marcos_a_agregar > 0)
        {

            if (marcos_libres >= marcos_a_agregar)
            {
                sem_wait(&sem_bitmap_marcos_libres);
                for (int i = 0; i < marcos_a_agregar; i++)
                {

                    int primer_marco_free = primer_marco_libre();
                    bitarray_set_bit(bitmap_marcos_libres, primer_marco_free);
                    list_add(aux->lista_paginas, primer_marco_free);
                }
                sem_post(&sem_bitmap_marcos_libres);
                aux->memoria_reservada = tamanio;
            }
            else
            {
                char *mensaje = string_new();
                string_append(&mensaje, "RESIZE OUT_OF_MEMORY");
                enviar_mensaje(mensaje, socket_cliente);
                return;
            }
        }
        else if (marcos_a_agregar < 0)
        {
            sem_wait(&sem_bitmap_marcos_libres);
            for (int i = 0; i > marcos_a_agregar; i--)
            {
                int tamanio_lista_marcos = list_size(aux->lista_paginas);
                int pagina_a_borrar = list_remove(aux->lista_paginas, (tamanio_lista_marcos - 1));
                bitarray_clean_bit(bitmap_marcos_libres, pagina_a_borrar);
            }
            sem_post(&sem_bitmap_marcos_libres);
            aux->memoria_reservada = tamanio;
        }
        char *mensaje = string_new();
        string_append(&mensaje, "RESIZE OK");
        enviar_mensaje(mensaje, socket_cliente);
    }
}

int cantidad_marcos_libres()
{
    int cantidad_total_marcos_libres = 0;
    for (int i = 0; bitmap_marcos_libres->size * 8 > i; i++)
    {
        if (!bitarray_test_bit(bitmap_marcos_libres, i))
        {
            cantidad_total_marcos_libres++;
        }
    }
    return cantidad_total_marcos_libres;
}

int cantidad_marcos_asignados(int pid)
{
    bool _es_pid_buscado(void *elemento)
    {
        return es_pid_buscado(pid, elemento);
    };
    t_instruccion_memoria *aux = list_find(memoria_instrucciones, _es_pid_buscado);
    return (list_size(aux->lista_paginas));
}

bool es_pid_buscado(int pid_buscado, void *elemento)
{
    t_instruccion_memoria *aux = malloc(sizeof(t_instruccion_memoria));
    aux = elemento;
    bool aux2 = (aux->process_id == pid_buscado);
    return (aux2);
}

int primer_marco_libre()
{
    int posicion = 0;
    while (bitarray_test_bit(bitmap_marcos_libres, posicion) && posicion < (bitmap_marcos_libres->size * 8 - 1))
    {
        posicion++;
    }

    if (posicion >= (bitmap_marcos_libres->size * 8 - 1))
    {
        posicion = -1;
    }
    return posicion;
}

void finalizar_proceso(int pid)
{
    bool _es_pid_buscado(void *elemento)
    {
        return es_pid_buscado(pid, elemento);
    };
    t_instruccion_memoria *aux = list_find(memoria_instrucciones, _es_pid_buscado);
    sem_wait(&sem_bitmap_marcos_libres);
    for (int i = 0; list_size(aux->lista_paginas) > i; i++)
    {
        int pagina_a_liberar = list_get(aux->lista_paginas, i);
        bitarray_clean_bit(bitmap_marcos_libres, pagina_a_liberar);
    }

    t_instruccion_memoria *aux2 = list_remove_by_condition(memoria_instrucciones, _es_pid_buscado);
    list_destroy(aux2->lista_instrucciones);
    list_destroy(aux2->lista_paginas);
    free(aux2);

    sem_post(&sem_bitmap_marcos_libres);
}

void obtener_marco(int pid, int nro_pagina, int socket_cliente)
{
    bool _es_pid_buscado(void *elemento)
    {
        return es_pid_buscado(pid, elemento);
    };
    t_instruccion_memoria *aux = list_find(memoria_instrucciones, _es_pid_buscado);

    if (list_size(aux->lista_paginas) <= nro_pagina)
    {
        char *mensaje = string_new();
        string_append(&mensaje, "OBTENER_MARCO SEGMENTATION_FAULT");
        enviar_mensaje(mensaje, socket_cliente);
    }
    else
    {
        int numero_pagina = list_get(aux->lista_paginas, nro_pagina);
        char *mensaje = string_new();
        string_append(&mensaje, "OBTENER_MARCO ");
        string_append(&mensaje, string_itoa(numero_pagina));
        enviar_mensaje(mensaje, socket_cliente);
    }
}

void leer(int direccion_fisica, int tamanio, int pid, int socket_cliente)
{

    char *mensaje = string_new();
    string_append(&mensaje, "LEER ");
    void *lectura = malloc(tamanio);
    memcpy(lectura, memoria + direccion_fisica, tamanio);
    void* aux = malloc(string_length(mensaje)+tamanio);
    memcpy(aux,mensaje,string_length(mensaje));
    memcpy(aux+string_length(mensaje),lectura, tamanio);
    enviar_mensaje(aux,socket_cliente);
}

void escribir(int direccion_fisica, int tamanio, int pid, void* valor, int socket_cliente)
{

    printf("LO QUE SE VA A ESCRIBIR");
    mem_hexdump(valor, tamanio);
    memcpy(memoria + direccion_fisica, valor, tamanio);
    char *mensaje = string_new();
    string_append(&mensaje, "ESCRIBIR OK");

    printf("LO QUE QUEDO");
    mem_hexdump(memoria, 200);
    //enviar_mensaje(mensaje, socket_cliente);
}

void copy_string (int df_origen, int df_destino, int tamanio ){
    memcpy (memoria + df_destino, memoria + df_origen, tamanio);
}