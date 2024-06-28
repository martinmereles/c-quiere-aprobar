#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <utils/client-server.h>
#include <utils/admin.h>
#include <utils/estructuras/estructuras.h>
#include <../include/instrucciones_cpu.h>
#include <../include/auxiliares.h>
#include <semaphore.h>
#include <../include/ciclo_instruccion.h>
#include <commons/temporal.h>


void iterator(char*); 

#endif
