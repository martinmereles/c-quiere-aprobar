#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include "typeDef.h"
#include "config.h"
#include "log.h"
#include "conexion.h"
#include "server.h"
#include "estructuras_comunes.h"
typedef enum
{
    KERNEL,
    CPU,
    ENTRADASALIDA,
    MEMORIA
} op_modulo;

typedef struct
{
    int socket_fd;
    t_log *logger;

} pthread_arg;

char *mi_funcion_compartida();

// handshake
int handshakeCliente(int socket_cliente, int modulo);
int handshakeServidor(int socket_cliente,  t_log *logger);

const char *op_ModuloToString(int opCode);
char* buscarEnConfig(t_config* config,char* index);


#endif