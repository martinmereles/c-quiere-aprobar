#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <utils/client-server.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <dirent.h>

extern char* nombre_interfaz;
extern char* tipo_interfaz;
extern int socket_cliente_memoria;
extern t_bitarray* bitmap_bloques_libres;
extern sem_t sem_fs;

typedef struct{

    char* nombre;
    int cantidad_bloques;
    int bloque_inicial;

}archivo_t;

void iniciar_hilo_kernel(t_config* config);
void entender_mensajes(char* mensaje, int socket_cliente,int tiempo_unidad_trabajo, t_config* config);
void io_gen_sleep(char* unidades_tiempo, int tiempo_unidad_trabajo);
void io_stdin_read(char** mensaje_split);
void io_stdout_write(char** mensaje_split);
void iniciar_dialfs(t_config * config);
void io_fs_create(char* nombre_archivo, t_config * config);
void io_fs_delete(char* nombre_archivo, t_config * config);
void io_fs_write(char** mensaje_split);
void io_fs_read(char** mensaje_split);
void io_fs_truncate(char* nombre_archivo, int tamanio_a_truncar, t_config * config);
int primer_bloque_libre();
void set_bloque_usado(int posicion, t_config * config);
void set_bloque_libre(int posicion, t_config * config);
int cantidad_bloques_contiguos(int bloque_final_archivo);
int cantidad_bloques_libres();

void compactar (char* no_compactar_arch, t_config* config);
bool esta_ordenado (archivo_t* element1, archivo_t* element2);
void limpiar_bitmap (t_config* config);
void escribir_bloques(char* palabra, int posicion);
void aviso_finalizar(char* pid, int socket_cliente);
void* recibir_desde_memoria(int socket_cliente);
#endif