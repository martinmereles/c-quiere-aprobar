#ifndef MANAGE_CLIENTS_H_
#define MANAGE_CLIENTS_H_
#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <commons/log.h>
#include <../include/main_kernel.h>

extern t_list* INTERFACES;
void atender_cliente_kernel(int socket_cliente);
void iniciar_hilo_server_kernel(int socket_cliente);


#endif