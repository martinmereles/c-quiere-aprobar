#ifndef UTILS_CLIENT_SERVER_H_
#define UTILS_CLIENT_SERVER_H_
#include <stdlib.h>
#include <stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>
#include<signal.h>
#include<unistd.h>

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

extern t_log* logger;

void* recibir_buffer(int*, int);

t_log* iniciar_logger(char *, char *);
t_config* iniciar_config(t_log*, char *);
void leer_consola(t_log*);
void paquete(int);
void terminar_programa(int, t_log*, t_config*);
int iniciar_servidor(char *);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);

int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);

#endif
