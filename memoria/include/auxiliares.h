#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/collections/list.h>

#include <../include/memoria_estructuras.h>
#include <utils/client-server.h>

#include <commons/bitarray.h>

extern t_list* memoria_instrucciones;
extern int retardo_respuesta;
extern void* memoria;
extern int tamanio_pagina;
extern t_bitarray * bitmap_marcos_libres;
extern sem_t *sem_bitmap_marcos_libres;

void iniciar_hilo_server_memoria(char* puerto);
void atender_cliente_memoria(int socket_cliente);
void iniciar_proceso(char* process_id, char* path);
void proxima_instruccion(char* process_id_find, char* program_counter, int socket_cliente);
void enviar_tamanio_pagina(int socket_cliente);
void resize (int tamanio, int pid, int socket_cliente);
int cantidad_marcos_libres();
int cantidad_marcos_asignados(int pid);
bool es_pid_buscado(int pid_buscado, void *elemento);
int primer_marco_libre();

#endif