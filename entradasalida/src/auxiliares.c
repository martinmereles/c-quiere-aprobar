#include "../include/auxiliares.h"

void iniciar_hilo_kernel(t_config *config)
{
    char *ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char *puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
    char *tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
    char *tiempo_unidad_trabajo_string = config_get_string_value(config, "TIEMPO_UNIDAD_TRABAJO");
    int tiempo_unidad_trabajo = atoi(tiempo_unidad_trabajo_string);
    log_info(logger, "La IP de Kernel es : %s", ip_kernel);
    log_info(logger, "El PUERTO de Kernel es : %s", puerto_kernel);
    char *nombre;
    nombre = readline("Ingrese el nombre de la interfaz > ");
    while (string_contains(nombre, " "))
    {
        log_info(logger, "No puede contener espacios");
        free(nombre);
        nombre = readline("Ingrese el nombre de la interfaz > ");
    }
    nombre_interfaz = malloc(sizeof(nombre));
    strcpy(nombre_interfaz, nombre);
    log_info(logger, "Se ingreso como nombre de la interfaz: %s", nombre_interfaz);

    int socket_cliente_kernel = crear_conexion(ip_kernel, puerto_kernel);
    char *msj_conexion = string_new();
    string_append(&msj_conexion, "CONECTAR_INTERFAZ ");
    string_append(&msj_conexion, tipo_interfaz);
    string_append(&msj_conexion, " ");
    string_append(&msj_conexion, nombre);
    enviar_mensaje(msj_conexion, socket_cliente_kernel);
    free(nombre);

    t_list *lista;
    while (1)
    {
        int cod_op = recibir_operacion(socket_cliente_kernel);
        ;
        switch (cod_op)
        {
        case MENSAJE:
            int size;
            char *buffer = recibir_buffer(&size, socket_cliente_kernel);
            log_info(logger, "Me llego el mensaje %s", buffer);
            entender_mensajes(buffer, socket_cliente_kernel, tiempo_unidad_trabajo, config);
            free(buffer);
            break;
        case PAQUETE:
            lista = recibir_paquete(socket_cliente_kernel);
            log_info(logger, "Me llegaron los siguientes valores:\n");
            list_iterate(lista, (void *)iterator);
            break;
        case -1:
            log_error(logger, "el cliente se desconecto.");
            return EXIT_FAILURE;
        default:
            log_warning(logger, "Operacion desconocida. No quieras meter la pata");
            break;
        }
    }
}

void entender_mensajes(char *mensaje, int socket_cliente, int tiempo_unidad_trabajo, t_config* config)
{
    char **mensaje_split = string_split(mensaje, " ");
    if (strcmp(tipo_interfaz, "GENERICA") == 0 && strcmp(mensaje_split[0], "IO_GEN_SLEEP") == 0)
    {
        io_gen_sleep(mensaje_split[1], tiempo_unidad_trabajo);
        aviso_finalizar(mensaje_split[2], socket_cliente);
    }
    else if (strcmp(tipo_interfaz, "STDIN") == 0 && strcmp(mensaje_split[0], "IO_STDIN_READ") == 0)
    {
        io_stdin_read(mensaje_split[1], mensaje_split[2]);
        aviso_finalizar(mensaje_split[3], socket_cliente);
    }
    else if (strcmp(tipo_interfaz, "STDOUT") == 0 && strcmp(mensaje_split[0], "IO_STDOUT_WRITE") == 0)
    {
        io_stdout_write(mensaje_split[1], mensaje_split[2]);
        aviso_finalizar(mensaje_split[3], socket_cliente);
    }
    else if (strcmp(tipo_interfaz, "DIALFS") == 0 && strcmp(mensaje_split[0], "IO_FS_CREATE") == 0)
    {
        io_fs_create(mensaje_split[1], config);
        aviso_finalizar(mensaje_split[2], socket_cliente);
    }
    else if (strcmp(tipo_interfaz, "DIALFS") == 0 && strcmp(mensaje_split[0], "IO_FS_DELETE") == 0)
    {
        io_fs_delete(mensaje_split[1], config);
        aviso_finalizar(mensaje_split[2], socket_cliente);
    }
    else if (strcmp(tipo_interfaz, "DIALFS") == 0 && strcmp(mensaje_split[0], "IO_FS_TRUNCATE") == 0)
    {
        io_fs_truncate(mensaje_split[1], atoi(mensaje_split[2]), config);
        aviso_finalizar(mensaje_split[3], socket_cliente);
    }
    else if (strcmp(tipo_interfaz, "DIALFS") == 0 && strcmp(mensaje_split[0], "IO_FS_WRITE") == 0)
    {
        io_fs_write(mensaje_split[1], mensaje_split[2], atoi(mensaje_split[3]), mensaje_split[4]);//revisar parametro 2 y 4
        aviso_finalizar(mensaje_split[5], socket_cliente);
    }
    else if (strcmp(tipo_interfaz, "DIALFS") == 0 && strcmp(mensaje_split[0], "IO_FS_READ") == 0)
    {
        io_fs_read(mensaje_split[1], mensaje_split[2], atoi(mensaje_split[3]), mensaje_split[4]);//revisar parametro 2 y 4
        aviso_finalizar(mensaje_split[5], socket_cliente);
    }
    else
    {
        log_error(logger, "Esta interfaz no entiende dicho mensaje");
    }
}

void aviso_finalizar(char* pid, int socket_cliente){

    char *mensaje_kernel = string_new();
    string_append(&mensaje_kernel, "FIN_IO ");
    string_append(&mensaje_kernel, nombre_interfaz);
    string_append(&mensaje_kernel, " ");
    string_append(&mensaje_kernel, pid);
    log_info(logger, "Se envia el mensaje=> %s", mensaje_kernel);
    enviar_mensaje(mensaje_kernel, socket_cliente);
}

void io_gen_sleep(char *unidades_tiempo, int tiempo_unidad_trabajo)
{
    int unidades_tiempo_int = atoi(unidades_tiempo);
    log_info(logger, "Se inicia tarea IO_GEN_SLEEP");
    usleep((tiempo_unidad_trabajo * unidades_tiempo_int) * 1000);
    log_info(logger, "Se termino tarea IO_GEN_SLEEP");
}

void io_stdin_read(char *direccion, char *tamanio)
{
    int tamanio_int = atoi(tamanio);
    char *texto = readline("Ingrese el texto para STDIN > ");
    char *texto_a_guardar = string_substring(texto, 0, tamanio_int);
    char *mensaje = string_new();
    string_append(&mensaje, "IO_STDIN_READ ");
    string_append(&mensaje, direccion);
    string_append(&mensaje, " ");
    string_append(&mensaje, texto_a_guardar);
    log_info(logger, "Se enviara el siguiente mensaje a memoria=> %s", mensaje);
    enviar_mensaje(mensaje, socket_cliente_memoria);
    log_info(logger, "Se termino tarea IO_STDIN_READ");
}

void io_stdout_write(char *direccion, char *tamanio)
{
    char *mensaje = string_new();
    string_append(&mensaje, "IO_STDOUT_WRITE ");
    string_append(&mensaje, direccion);
    string_append(&mensaje, " ");
    string_append(&mensaje, tamanio);
    log_info(logger, "Se enviara el siguiente mensaje a memoria=> %s", mensaje);
    enviar_mensaje(mensaje, socket_cliente_memoria);
    int cod_op = recibir_operacion(socket_cliente_memoria);
    switch (cod_op)
    {
    case MENSAJE:
        int size;
        char *buffer = recibir_buffer(&size, socket_cliente_memoria);
        printf("Se recibio desde memoria el mensaje=> %s", buffer);
        free(buffer);
        break;
    default:
        log_warning(logger, "Operacion desconocida. No quieras meter la pata");
        break;
    }
    log_info(logger, "Se termino tarea IO_STDOUT_WRITE");
}

void iniciar_dialfs(t_config *config)
{
    char *path_dialfs = config_get_string_value(config, "PATH_BASE_DIALFS");
    char *path_dialfs_bloques = string_new();
    char *path_dialfs_bitmap = string_new();
    string_append(&path_dialfs_bloques, path_dialfs);
    string_append(&path_dialfs_bitmap, path_dialfs);
    string_append(&path_dialfs_bloques, "/");
    string_append(&path_dialfs_bitmap, "/");
    string_append(&path_dialfs_bloques, "bloques.dat");
    string_append(&path_dialfs_bitmap, "bitmap.dat");
    int tamanio_bloque = config_get_int_value(config, "BLOCK_SIZE");
    int cantidad_bloques = config_get_int_value(config, "BLOCK_COUNT");

    FILE *file_bitmap = fopen(path_dialfs_bitmap, "r+");
    FILE *file_bloques = fopen(path_dialfs_bloques, "r+");

    if (file_bitmap == NULL || file_bloques == NULL)
    {
        file_bloques = fopen(path_dialfs_bloques, "w+");
        file_bitmap = fopen(path_dialfs_bitmap, "w+");

        // Inicializo bitmap
        void *bitmap = malloc(cantidad_bloques / 8);
        bitmap_bloques_libres = bitarray_create_with_mode(bitmap, cantidad_bloques / 8, LSB_FIRST);
        for (int i = 0; i < cantidad_bloques; i++)
        {
            bitarray_clean_bit(bitmap_bloques_libres, i);
        }
        fwrite(bitmap_bloques_libres->bitarray, bitmap_bloques_libres->size, 1, file_bitmap);

        // Inicializo bloques
        void *bloques = calloc(cantidad_bloques, tamanio_bloque);
        fwrite(bloques, tamanio_bloque, cantidad_bloques, file_bloques);

        fclose(file_bitmap);
        fclose(file_bloques);
    }
    else
    {
        fseek(file_bitmap, 0L, SEEK_END);
        int sz = ftell(file_bitmap);
        rewind(file_bitmap);

        void *bitmap = malloc(cantidad_bloques / 8);
        bitmap_bloques_libres = bitarray_create_with_mode(bitmap, cantidad_bloques / 8, LSB_FIRST);
        fread(bitmap_bloques_libres->bitarray, sz, 1, file_bitmap);
        fclose(file_bitmap);
        fclose(file_bloques);
    }
}

void io_fs_create(char *nombre_archivo, t_config *config)
{
    char *path_dialfs = config_get_string_value(config, "PATH_BASE_DIALFS");
    char *path_metadata = string_new();
    string_append(&path_metadata, path_dialfs);
    string_append(&path_metadata, "/");
    string_append(&path_metadata, nombre_archivo);
    char *text_bloque_inicial = string_new();
    char *text_tamanio = string_new();
    string_append(&text_bloque_inicial, "BLOQUE_INICIAL=");
    string_append(&text_tamanio, "TAMANIO_ARCHIVO=0");

    sem_wait(&sem_fs);
    int posicion = primer_bloque_libre();
    if (posicion != -1)
    {
        FILE *metadata = txt_open_for_append(path_metadata);
        string_append(&text_bloque_inicial, string_itoa(posicion));
        string_append(&text_bloque_inicial, "\n");
        txt_write_in_file(metadata, text_bloque_inicial);
        txt_write_in_file(metadata, text_tamanio);
        set_bloque_usado(posicion, config);
        txt_close_file(metadata);
    }
    else
    {
        log_error(logger, "No hay espacio para la creacion");
    }
    sem_post(&sem_fs);
}

void io_fs_delete(char *nombre_archivo, t_config *config)
{

    char *path_dialfs = config_get_string_value(config, "PATH_BASE_DIALFS");
    char *path_metadata = string_new();
    int tamanio_bloque = config_get_int_value(config, "BLOCK_SIZE");
    string_append(&path_metadata, path_dialfs);
    string_append(&path_metadata, "/");
    string_append(&path_metadata, nombre_archivo);

    sem_wait(&sem_fs);
    t_config *archivo_metadata;
    archivo_metadata = iniciar_config(logger, path_metadata);
    int tamanio_archivo = config_get_int_value(archivo_metadata, "TAMANIO_ARCHIVO");
    int bloque_inicial = config_get_int_value(archivo_metadata, "BLOQUE_INICIAL");
    int cant_bloques_actuales = floor(tamanio_archivo / tamanio_bloque) + 1;

    for (int i = 0; i < cant_bloques_actuales; i++)
    {
        set_bloque_libre(bloque_inicial + i, config);
    }

    config_destroy(archivo_metadata);
    remove(path_metadata);
    sem_post(&sem_fs);
}


void io_fs_write(char* nombre_archivo, int registro_direccion,  int tamanio, int registro_puntero){

}

void io_fs_read(char* nombre_archivo, int registro_direccion,  int tamanio, int registro_puntero){

}


void io_fs_truncate(char *nombre_archivo, int tamanio_a_truncar, t_config *config)
{
    char *path_dialfs = config_get_string_value(config, "PATH_BASE_DIALFS");
    char *path_metadata = string_new();
    int tamanio_bloque = config_get_int_value(config, "BLOCK_SIZE");
    string_append(&path_metadata, path_dialfs);
    string_append(&path_metadata, "/");
    string_append(&path_metadata, nombre_archivo);

    // Preguntar si esta bien usarlo
    sem_wait(&sem_fs);

    t_config *archivo_metadata;
    archivo_metadata = iniciar_config(logger, path_metadata);
    int tamanio_archivo = config_get_int_value(archivo_metadata, "TAMANIO_ARCHIVO");
    int bloque_inicial = config_get_int_value(archivo_metadata, "BLOQUE_INICIAL");

    int cant_bloques_actuales = floor(tamanio_archivo / tamanio_bloque) + 1;
    int cant_bloques_nuevos = floor(tamanio_a_truncar / tamanio_bloque) + 1;

    if (cant_bloques_actuales == cant_bloques_nuevos)
    {
        config_set_value(archivo_metadata, "TAMANIO_ARCHIVO", string_itoa(tamanio_a_truncar));
        config_save(archivo_metadata);
        config_destroy(archivo_metadata);
    }
    else if (cant_bloques_actuales > cant_bloques_nuevos)
    {
        config_set_value(archivo_metadata, "TAMANIO_ARCHIVO", string_itoa(tamanio_a_truncar));
        config_save(archivo_metadata);
        config_destroy(archivo_metadata);

        int cant_a_liberar = cant_bloques_actuales - cant_bloques_nuevos;
        for (int i = 1; i <= cant_a_liberar; i++)
        {
            set_bloque_libre(bloque_inicial + cant_bloques_nuevos + i - 1, config);
        }
    }
    else if (cant_bloques_actuales < cant_bloques_nuevos)
    {
        int cantidad_total_bloques_libres = cantidad_bloques_libres();
        if (cantidad_total_bloques_libres >= (cant_bloques_nuevos - cant_bloques_actuales))
        {
            int bloque_final_archivo = cant_bloques_actuales + bloque_inicial - 1;
            int cantidad_libres_contiguos = cantidad_bloques_contiguos(bloque_final_archivo);
            int cantidad_a_agregar = cant_bloques_nuevos - cant_bloques_actuales;
            if (cantidad_libres_contiguos >= cantidad_a_agregar)
            {
                for (int i = 1; i <= cantidad_a_agregar; i++)
                {
                    set_bloque_usado(bloque_final_archivo + i, config);
                }
                config_set_value(archivo_metadata, "TAMANIO_ARCHIVO", string_itoa(tamanio_a_truncar));
            }
            else
            {   
                char *path_bloques = string_new();
                string_append(&path_bloques, path_dialfs);
                string_append(&path_bloques, "/bloques.dat");

                void *archivo_data = malloc(tamanio_bloque * cant_bloques_actuales);

                // Abro archivo
                FILE *fs_archivos_bloques = fopen(path_bloques, "r+");
                // Me posiciono en el bloque inicial
                fseek(fs_archivos_bloques, tamanio_bloque * bloque_inicial, SEEK_SET);
                // Leo la cantidad de bloques del archivo
                fread(archivo_data, tamanio_bloque, cant_bloques_actuales, fs_archivos_bloques);

                int i = bloque_inicial;
                for (i; i <= (bloque_inicial + cant_bloques_actuales - 1); i++){
                    set_bloque_libre(i, config);
                }

                compactar(nombre_archivo, config);

                int primer_bloque_free = primer_bloque_libre();
                
                config_set_value(archivo_metadata, "TAMANIO_ARCHIVO", string_itoa(tamanio_a_truncar));
                config_set_value(archivo_metadata, "BLOQUE_INICIAL", string_itoa(primer_bloque_free));

                // Me posiciono en el primer bloque libre
                fseek(fs_archivos_bloques, tamanio_bloque * primer_bloque_free, SEEK_SET);
                // Escribo a partir del primer bloque libre lo que se leyo en archivo_data
                fwrite(archivo_data, tamanio_bloque, cant_bloques_actuales, fs_archivos_bloques);
                
                int j = primer_bloque_free;
                for (j; j <= (primer_bloque_free + cant_bloques_nuevos - 1); j++){
                    set_bloque_usado(j, config);
                }

                fclose(fs_archivos_bloques);
                free(archivo_data);
            }
            config_save(archivo_metadata);
            config_destroy(archivo_metadata);
        }
        else
        {
            log_error(logger, "Espacio insuficiente en disco");
        }
    }

    sem_post(&sem_fs);
}

int cantidad_bloques_contiguos(int bloque_final_archivo)
{
    int cantidad_libres_contiguos = 0;
    while ((bloque_final_archivo + cantidad_libres_contiguos + 1) <= (bitmap_bloques_libres->size * 8 - 1) && !bitarray_test_bit(bitmap_bloques_libres, (bloque_final_archivo + cantidad_libres_contiguos + 1)))
    {
        cantidad_libres_contiguos++;
    }
    return (cantidad_libres_contiguos);
}

int cantidad_bloques_libres()
{
    int cantidad_total_bloques_libres = 0;
    for (int i = 0; bitmap_bloques_libres->size * 8 > i; i++)
    {
        if (!bitarray_test_bit(bitmap_bloques_libres, i))
        {
            cantidad_total_bloques_libres++;
        }
    }
    return cantidad_total_bloques_libres;
}

int primer_bloque_libre()
{
    int posicion = 0;
    while (bitarray_test_bit(bitmap_bloques_libres, posicion) && posicion < (bitmap_bloques_libres->size * 8 - 1))
    {
        posicion++;
    }

    if (posicion >= (bitmap_bloques_libres->size * 8 - 1))
    {
        posicion = -1;
    }

    return posicion;
}

void set_bloque_usado(int posicion, t_config *config)
{
    char *path_dialfs = config_get_string_value(config, "PATH_BASE_DIALFS");
    char *path_dialfs_bitmap = string_new();
    string_append(&path_dialfs_bitmap, path_dialfs);
    string_append(&path_dialfs_bitmap, "/");
    string_append(&path_dialfs_bitmap, "bitmap.dat");
    FILE* file_bitmap = fopen(path_dialfs_bitmap, "r+");
    bitarray_set_bit(bitmap_bloques_libres, posicion);
    fwrite(bitmap_bloques_libres->bitarray, bitmap_bloques_libres->size, 1, file_bitmap);
    fclose(file_bitmap);
}

void set_bloque_libre(int posicion, t_config * config){
    char* path_dialfs = config_get_string_value(config, "PATH_BASE_DIALFS");
    char* path_dialfs_bitmap = string_new();
    string_append(&path_dialfs_bitmap, path_dialfs);
    string_append(&path_dialfs_bitmap, "/");
    string_append(&path_dialfs_bitmap, "bitmap.dat");
    FILE* file_bitmap = fopen(path_dialfs_bitmap, "r+");
    bitarray_clean_bit(bitmap_bloques_libres, posicion);
    fwrite(bitmap_bloques_libres->bitarray, bitmap_bloques_libres->size, 1, file_bitmap);
    fclose(file_bitmap);
}

bool esta_ordenado(archivo_t *element1, archivo_t *element2)
{
    bool resultado = element1->bloque_inicial < element2->bloque_inicial;
    return resultado;
}

void limpiar_bitmap(t_config *config)
{
    for (int i = 0; i < bitarray_get_max_bit(bitmap_bloques_libres); i++)
    {
        set_bloque_libre(i, config);
    }
}

void compactar(char* no_compactar_arch, t_config *config)
{
    char *path_dialfs = config_get_string_value(config, "PATH_BASE_DIALFS");
    int retraso_compactacion = config_get_int_value(config, "RETRASO_COMPACTACION");
    int tamanio_bloque = config_get_int_value(config, "BLOCK_SIZE");
    t_list *lista_archivos = list_create();
    char *path_bloques = string_new();
    string_append(&path_bloques, path_dialfs);
    string_append(&path_bloques, "/");
    string_append(&path_bloques, "bloques.dat");

    DIR *d;
    struct dirent *dir;
    d = opendir(path_dialfs);

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (!string_starts_with(dir->d_name, "bitmap.dat") && !string_starts_with(dir->d_name, "bloques.dat") && !string_starts_with(dir->d_name, ".") && !string_starts_with(dir->d_name, no_compactar_arch))
            {
                archivo_t *aux = malloc(sizeof(archivo_t));

                aux->nombre = string_duplicate(dir->d_name);
                int a = string_length(aux->nombre);

                t_config *archivo_metadata;
                char *path_metadata = string_new();
                string_append(&path_metadata, path_dialfs);
                string_append(&path_metadata, "/");
                string_append(&path_metadata, aux->nombre);

                archivo_metadata = iniciar_config(logger, path_metadata);
                int tamanio_archivo = config_get_int_value(archivo_metadata, "TAMANIO_ARCHIVO");
                int bloque_inicial = config_get_int_value(archivo_metadata, "BLOQUE_INICIAL");

                aux->bloque_inicial = bloque_inicial;
                int cant_bloques_actuales = floor(tamanio_archivo / tamanio_bloque) + 1;
                aux->cantidad_bloques = cant_bloques_actuales;

                list_add_sorted(lista_archivos, aux, (void *)esta_ordenado);
                config_destroy(archivo_metadata);
            }
        }
        closedir(d);
    }

    limpiar_bitmap(config);
    int cantidad_archivos = list_size(lista_archivos);

    for (int i = 0; i < cantidad_archivos; i++)
    {
        archivo_t *auxiliar = malloc(sizeof(archivo_t));
        auxiliar = list_remove(lista_archivos, 0);

        t_config *metadata;

        char *path_metadata_auxiliar = string_new();
        string_append(&path_metadata_auxiliar, path_dialfs);
        string_append(&path_metadata_auxiliar, "/");
        string_append(&path_metadata_auxiliar, auxiliar->nombre);

        metadata = iniciar_config(logger, path_metadata_auxiliar);

        int primer_bloque_free = primer_bloque_libre();

        config_set_value(metadata, "BLOQUE_INICIAL", string_itoa(primer_bloque_free));
        config_save(metadata);
        config_destroy(metadata);
        void *archivo_data = malloc(tamanio_bloque * auxiliar->cantidad_bloques);

        // Abro archivo
        FILE *fs_archivos_bloques = fopen(path_bloques, "r+");
        // Me posiciono en el bloque inicial
        fseek(fs_archivos_bloques, tamanio_bloque * auxiliar->bloque_inicial, SEEK_SET);
        // Leo la cantidad de bloques del archivo
        fread(archivo_data, tamanio_bloque, auxiliar->cantidad_bloques, fs_archivos_bloques);
        // Me posiciono en el primer bloque libre
        fseek(fs_archivos_bloques, tamanio_bloque * primer_bloque_free, SEEK_SET);
        // Escribo a partir del primer bloque libre lo que se leyo en archivo_data
        fwrite(archivo_data, tamanio_bloque, auxiliar->cantidad_bloques, fs_archivos_bloques);
        int i = primer_bloque_free;
        for (i; i <= (primer_bloque_free + auxiliar->cantidad_bloques - 1); i++)
        {
            set_bloque_usado(i, config);
        }

        fclose(fs_archivos_bloques);
        free(auxiliar);
    }
    //usleep(retraso_compactacion * 1000);
}

void escribir_bloques(char *palabra, int posicion)
{

    FILE *file_bloques = fopen("/home/utnso/dialfs/bloques.dat", "r+");
    fseek(file_bloques, 64 * posicion, SEEK_SET);
    fwrite(palabra, string_length(palabra), 1, file_bloques);
    fclose(file_bloques);
}