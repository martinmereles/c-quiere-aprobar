#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <utils/client-server.h>

extern char* nombre_interfaz;
extern char* tipo_interfaz;
extern int socket_cliente_memoria;

void iniciar_hilo_kernel(t_config* config);
void entender_mensajes(char* mensaje, int socket_cliente,int tiempo_unidad_trabajo);
void io_gen_sleep(char* unidades_tiempo, char* pid, int tiempo_unidad_trabajo, int socket_cliente);
void io_stdin_read(char *direccion, char *tamanio, char * pid, int socket_cliente);
void io_stdout_write(char *direccion, char *tamanio, char * pid, int socket_cliente);

#endif