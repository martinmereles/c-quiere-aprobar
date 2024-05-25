#ifndef AUXILIARES_H_
#define AUXILIARES_H_

#include <utils/client-server.h>
#include <../include/main_cpu.h>

extern char* instruccion_exec;

void atender_cliente_cpu(int socket_cliente);
#endif