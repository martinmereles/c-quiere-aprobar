#include <utils/client-server.h>
#include <commons/config.h>
#include <commons/log.h>

t_log* iniciar_logger(char * archivo, char * nombre)
{
    t_log* logger_nuevo = log_create(archivo, nombre, true, LOG_LEVEL_INFO);

	return logger_nuevo;
}

t_config* iniciar_config(t_log* logger, char * archivo)
{
	t_config* nuevo_config;

    nuevo_config = config_create(archivo);

    if (nuevo_config==NULL){
        log_info(logger, "No existe el archivo %s", archivo);
        exit(EXIT_FAILURE);
    }

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* command;
    command = readline("> ");
    while(strcmp(command,"")){
    	log_info(logger, command);
    	free(command);
    	command = readline("> ");
    }
    free(command);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
    char* command;
    command = readline("> ");
    while(strcmp(command,"")){
    log_info(logger, command);
    agregar_a_paquete(paquete,command,strlen(command)+1);
    free(command);
    command = readline("> ");
    }
    free(command);
    enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

int iniciar_servidor(char * puerto)
{
	int socket_servidor;
    int err;

	struct addrinfo hints, *server_info, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL, puerto, &hints, &server_info);
	log_info(logger, "getaddrinfo %d", err);

	// Creamos el socket de escucha del servidor
    socket_servidor = socket(server_info->ai_family,
                            server_info->ai_socktype,
                            server_info->ai_protocol);
	log_info(logger, "socket_servidor %d", socket_servidor);
	
	//Para reutilizar socket sin esperar
	int verdadero = 1;
	if (setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEPORT, &verdadero, sizeof(int)) < 0) 
    error("setsockopt(SO_REUSEADDR) failed");

	// Asociamos el socket a un puerto
    err = bind(socket_servidor,server_info->ai_addr, server_info->ai_addrlen);
	log_info(logger, "Bind %d", err);

	// Escuchamos las conexiones entrantes
    err = listen(socket_servidor, SOMAXCONN);
	log_info(logger, "Listen %d", err);

	freeaddrinfo(server_info);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor,NULL,NULL);
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void* recibir_buffer_pcb(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	

	pcb_t * pcb = malloc (sizeof(pcb_t));
	pcb->reg_generales = malloc(sizeof(registros_t));

	memcpy(pcb, buffer+4, sizeof(pcb_t));
	memcpy(pcb->reg_generales, buffer+8+sizeof(pcb_t), sizeof(registros_t));

	return pcb;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;
    int err;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = 0;
    socket_cliente = socket(server_info->ai_family,
                            server_info->ai_socktype,
                            server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
    err = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);
	log_info(logger, "El socket_cliente es : %d", socket_cliente);
	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

void iterator(char* value) 
{
	log_info(logger,"%s", value);
}



void iniciar_hilo_server(char* puerto){
int socket_servidor = iniciar_servidor(puerto);
while (1) {
     pthread_t hiloAtencion;
     int *socket_cliente = (int)malloc(sizeof(int));
     socket_cliente = esperar_cliente(socket_servidor);
     pthread_create(&hiloAtencion,
                    NULL,
                    (void*) atender_cliente,
                    socket_cliente);
     pthread_detach(hiloAtencion);
	}
}
	


void atender_cliente(int socket_cliente){
	t_list* lista;
	while (1) {
		log_info(logger, "El SOCKET INTERRUPT cliente es : %d", socket_cliente);
		int cod_op = recibir_operacion(socket_cliente);; 
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(socket_cliente);
			break;
		case PAQUETE:
			lista = recibir_paquete(socket_cliente );
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto.");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}

void enviar_pcb_contexto(int socket_destino, pcb_t* pcb_a_enviar){

    t_paquete* paquete_pcb = crear_paquete();
    agregar_a_paquete(paquete_pcb, pcb_a_enviar, sizeof(pcb_t));
    agregar_a_paquete(paquete_pcb, pcb_a_enviar->reg_generales, sizeof(registros_t));

    log_info (logger, "Se enviara el PCB con id: %d al socket %d", pcb_a_enviar->pid, socket_destino);
    
    enviar_paquete(paquete_pcb, socket_destino);    

    eliminar_paquete (paquete_pcb);
}

void enviar_pcb_contexto_motivo(int socket_destino, pcb_t* pcb_a_enviar, char* motivo){


    t_paquete* paquete_pcb = crear_paquete();
    agregar_a_paquete(paquete_pcb, pcb_a_enviar, sizeof(pcb_t));
    agregar_a_paquete(paquete_pcb, pcb_a_enviar->reg_generales, sizeof(registros_t));
	
	agregar_a_paquete(paquete_pcb, motivo, string_length(motivo));

    log_info (logger, "Se enviara el PCB y el motivo con id: %d al socket %d", pcb_a_enviar->pid, socket_destino);
    
    enviar_paquete(paquete_pcb, socket_destino);    

    eliminar_paquete (paquete_pcb);
}

void* recibir_buffer_pcb_motivo(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	

	pcb_t * pcb = malloc (sizeof(pcb_t));
	pcb->reg_generales = malloc(sizeof(registros_t));

	memcpy(pcb, buffer+4, sizeof(pcb_t));
	memcpy(pcb->reg_generales, buffer+8+sizeof(pcb_t), sizeof(registros_t));
	int tamanio_motivo;
	int peso_pcb = sizeof(pcb_t);
	int peso_registros = sizeof(registros_t);
	log_info(logger, "el PCB pesa %d, y los regitros pesan %d", peso_pcb, peso_registros);
	log_info(logger, "un int pesa %d", sizeof(int));

	memcpy(&tamanio_motivo, buffer+68, sizeof(int));
	char* motivo_recibido = malloc(5);
	memcpy(motivo_recibido, buffer+12+sizeof(pcb_t) + sizeof(registros_t), 5);




	return pcb;
}