#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <utils/client-server.h>

void iniciar_hilo_kernel(t_config* config);
void entender_mensajes(char* mensaje, int socket_cliente,double tiempo_unidad_trabajo);
void io_gen_sleep(char* unidades_tiempo, double tiempo_unidad_trabajo, int socket_cliente);

#endif