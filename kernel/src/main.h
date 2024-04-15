#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>
#include<signal.h>
#include<unistd.h>

t_log* iniciar_logger(void);
t_config* iniciar_config(t_log*);
void leer_consola(t_log*);
void terminar_programa(int, t_log*, t_config*);

#endif