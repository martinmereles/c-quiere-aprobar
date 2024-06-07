#ifndef MANAGE_CLIENTS_H_
#define MANAGE_CLIENTS_H_
#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <commons/log.h>
#include <../include/main_kernel.h>

extern t_list* INTERFACES;
void atender_cliente_kernel(int socket_cliente);
void iniciar_hilo_server_kernel(char* socket_cliente);
void conectar_interfaz(char* tipo_interfaz, char* identificador, int socket_interfaz);
bool es_interfaz_buscada(char* identificador, void* elemento);
void io_gen_sleep(char * interfaz, char* unidad_tiempo, int socket_cliente);

#endif