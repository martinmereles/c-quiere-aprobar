#ifndef MANAGE_CLIENTS_H_
#define MANAGE_CLIENTS_H_
#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <commons/log.h>
#include <../include/main_kernel.h>
#include <utils/estructuras/estructuras.h>
#include <../include/recursos.h>

extern t_list* INTERFACES;
extern int socket_memoria;
extern int socket_cpu_interrupt;
extern sem_t mutex_lista_interfaces;

void atender_cliente_kernel(int socket_cliente);
void iniciar_hilo_server_kernel(char* socket_cliente);

void conectar_interfaz(char* tipo_interfaz, char* identificador, int socket_interfaz);
bool es_interfaz_buscada(char* identificador, void* elemento);

void io_gen_sleep(char * interfaz, char* unidad_tiempo, char* pid);
void io_stdin_read(char * interfaz, char* direccion, char* tamanio, char *pid);
void io_stdout_write(char * interfaz, char* direccion, char* tamanio, char *pid);
void io_fs_create(char * interfaz, char* nombre_archivo, char *pid);
void io_fs_delete(char * interfaz, char* nombre_archivo, char *pid);
void io_fs_truncate(char * interfaz, char* nombre_archivo, char* tamanio_a_truncar, char *pid);
void io_fs_write(char * interfaz, char* nombre_archivo, char* direccion, char* tamanio, char* puntero_archivo, char *pid);
void io_fs_read(char * interfaz, char* nombre_archivo, char* direccion, char* tamanio, char* puntero_archivo, char *pid);

void liberar_interfaz(char * interfaz, char* pid, char* algoritmo);
bool es_interfaz_buscada_socket (int socket, void* elemento);
void consumidor_interfaz(t_interfaz* interfaz);
void desconectar_interfaz(int socket_cliente);


bool admite_instruccion(char *tipo, char *instruccion);

#endif