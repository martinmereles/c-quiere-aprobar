#ifndef INSTRUCCIONES_CPU_H
#define INSTRUCCIONES_CPU_H
#include <utils/estructuras/estructuras.h>
#include <stdint.h>
#include <commons/log.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <utils/client-server.h>
#include <commons/temporal.h>

extern registros_t *reg;
extern t_log* logger;
extern char* instruccion_exec;
extern pcb_t* contexto;
extern t_temporal* temporizador;

void ejecutarSentencia(int socket_kernel, int socket_cliente_memoria);
void set(char * registro, char * valor);
void sum(char * registroDestino, char * registroOrigen);
void sub(char * registroDestino, char * registroOrigen);
void jnz(char * registro, char * instruccion);
void io_gen_sleep(char * interfaz, char * unidadesDeTrabajo, int socket_kernel);
void exit_inst(int socket_kernel);
void mov_in();
void mov_out();
void resize();
void copy_string();
void wait();
void signal_instruccion();
void io_fs_create();
void io_fs_delete();
void io_fs_truncate();
void io_fs_write();
void io_fs_read();

#endif