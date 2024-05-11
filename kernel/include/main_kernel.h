#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <commons/collections/list.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <utils/admin.h>
#include <utils/client-server.h>
#include <commons/config.h>
#include <commons/log.h>
#include <readline/readline.h>
#include <utils/estructuras/estructuras.h>

void iterator(char*);
void atender_clientes(void *arg);

#endif