#ifndef SERVER_H
#define SERVER_H

#include "typeDef.h"#include"estructuras_comunes.h"

void iterator(char *value, t_log *logger);

void *recibir_buffer(int *, int);

int iniciar_servidor(const char *puerto, t_log *logger);
int esperar_cliente(int socket_servidor, t_log *logger);
t_list *recibir_paquete(int);
void recibir_mensaje(int socket_cliente, t_log *logger);
int recibir_operacion(int socket_cliente);

#endif