#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/collections/list.h>

#include <../include/memoria_estructuras.h>
#include <utils/client-server.h>

extern t_list* lista_instrucciones;

void iniciar_hilo_server_memoria(char* puerto);
void atender_cliente_memoria(int socket_cliente);
void iniciar_proceso(char* process_id, char* path);
char* proxima_instruccion(char* process_id_find, char* program_counter);
#endif