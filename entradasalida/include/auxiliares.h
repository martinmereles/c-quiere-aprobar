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

extern char* nombre_interfaz;
extern char* tipo_interfaz;
extern int socket_cliente_memoria;
extern t_bitarray* bitmap_bloques_libres;
extern sem_t sem_fs;

void iniciar_hilo_kernel(t_config* config);
void entender_mensajes(char* mensaje, int socket_cliente,int tiempo_unidad_trabajo);
void io_gen_sleep(char* unidades_tiempo, char* pid, int tiempo_unidad_trabajo, int socket_cliente);
void io_stdin_read(char *direccion, char *tamanio, char * pid, int socket_cliente);
void io_stdout_write(char *direccion, char *tamanio, char * pid, int socket_cliente);
void iniciar_dialfs(t_config * config);
void io_fs_create(char* nombre_archivo, t_config * config);
int primer_bloque_libre();
void set_bloque_usado(int posicion, t_config * config);

#endif